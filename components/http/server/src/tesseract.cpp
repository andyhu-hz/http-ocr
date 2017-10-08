#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include <iostream>
#include <sstream>
#include "tesseract.h"

#include <unistd.h>
string get_current_directory()
{
	char path[FILENAME_MAX];
	getcwd(path, sizeof(path));
	return string(path);
}

string ConvertPngToText(string filename)
{
	char *outText;
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	//api->SetVariable("textord_show_blobs", "true");
	//api->Init(NULL, "eng", tesseract::OEM_TESSERACT_ONLY);
	api->Init(NULL, "eng", tesseract::OEM_TESSERACT_LSTM_COMBINED);

	//api->SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);

	string file_pathname = get_current_directory() + "/" + filename;
	cout << "file_pathname = " << file_pathname << endl;

	Pix *image = pixRead(file_pathname.c_str());
	api->SetImage(image);
	api->SetVariable("save_blob_choices", "T");
	api->SetVariable("matcher_bad_match_pad", "0.2");
	//api->SetRectangle(37, 228, 548, 31);
	api->Recognize(0);
	tesseract::ResultIterator* ri = api->GetIterator();
	tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;
	//FILE* fout = fopen("txt_file.txt", "w");
	outText = api->GetUTF8Text();
	int conf = api->MeanTextConf();
	printf(outText);
	
	if (ri != 0)
	{
		do
		{
			const char* symbol = ri->GetUTF8Text(level);
			float conf = ri->Confidence(level);
			if (symbol != 0)
			{
				//printf("best choice is  %s conf: %f\n", symbol, conf);				

				/*
				printf("other choices are\n");
				tesseract::ChoiceIterator ci(*ri);
				do
				{
					printf("\t\t\t");
					const char* choice = ci.GetUTF8Text();
					printf("%s conf: %f\n", choice, ci.Confidence());
					//indent = true;
				} while (ci.Next());
				*/
			}
			delete[] symbol;
		} while (ri->Next(level));
	}	
	return string(outText);
}