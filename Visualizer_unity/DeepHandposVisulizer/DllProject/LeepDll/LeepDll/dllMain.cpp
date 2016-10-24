#include <stdio.h>
#include <string>
#include <Windows.h>
#include <Leap.h>

#include "leapMotion.h"
//#include "openCV.h"

#define EXPORT_API __declspec(dllexport)
#define GEST_COUNT 10

using namespace std;

extern "C"{
	LeapMotion leap;

	void EXPORT_API TEST(){
		MessageBox(NULL, L"Client DLL Test Successs", L"TEST", MB_OK);
	}

	void EXPORT_API Init(){
		leap.Init();
	}

	void EXPORT_API CalcProbabilty(float prob[]){
		//TO-DO

		for (int i = 0; i < GEST_COUNT; i++){
			if (i == 3)
				prob[i] = 1.0f;
			else
				prob[i] = 0.0f;
		}
	}

	void EXPORT_API LeapUpdataFrame(){
		leap.updateFrame();
	}

	void EXPORT_API LeapShowImage(unsigned char left[], unsigned char right[], int width_height){
		for (int i = 0; i < 2; i++){
			cv::Mat Image(HEIGHT, WIDTH, CV_8UC1, leap.imgdata.data[i]);
			cv::resize(Image, Image, cv::Size(width_height, width_height));

			for (int j = 0; j < width_height * width_height; j++){
				if (i == 0){
					left[j] = Image.at<unsigned char>(j);
				}
				else{
					right[j] = Image.at<unsigned char>(j);
				}
			}

			char id[10];
			itoa(i, id, 10);
			cv::imshow(id, Image);
		}
		cv::waitKey(1);
	}

	int EXPORT_API getPalmData(float pos[], float norm[], float dir[]){
		int check = -1;

		for (int i = 0; i < 2; i++){
			if (!leap.handsdata[i].state)
				continue;

			if (!leap.handsdata[i].isLeft){
				check = 1;
				for (int j = 0; j < 3; j++){
					pos[j] = leap.handsdata[i].hand_palmPosition[j];
					norm[j] = leap.handsdata[i].hand_palmNormal[j];
					dir[j] = leap.handsdata[i].hand_direction[j];
				}
			}
		}

		return check;
	}

	void EXPORT_API getFingerData(float next[], float prev[]){
		for (int h = 0; h < 2; h++){
			if (leap.handsdata[h].state && !leap.handsdata[h].isLeft){
				for (int f = 0; f < 5; f++){				//finger idx
					for (int b = 0; b < 4; b++){			//bon idx
						for (int k = 0; k < 3; k++){
							next[f * 4 * 3 + b * 3 + k] = leap.handsdata[h].finger[f].bone[b][1][k];
							prev[f * 4 * 3 + b * 3 + k] = leap.handsdata[h].finger[f].bone[b][0][k];
						}
					}
				}
			}
		}
	}

}