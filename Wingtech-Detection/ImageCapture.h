#pragma once
#include <QThread>
#include "windows.h"
#include <QMutex>
#include "MvCamera.h"
#include "opencv2/opencv.hpp"
#include "qdebug.h"
using namespace std;
using namespace cv;

typedef enum
{
	CAMERA_FIRST,
	CAMERA_SECOND,
	CAMERA_THIRD,
	CAMERA_FOURTH
}e_CameraType;

typedef enum
{
	RUN_ONLINE,  //�������У����㷨��ͳ�ƽ����PLCͨѶ
	CAMEREA_TEST, //������ԣ���ʾ���ͼ�񣬲����㷨
	ONLINE_ALGO_TEST, //���߲����㷨�������ͼ�����㷨����ʾ���
	OFFLINE_ALGO_TEST //��������ͼ�����㷨����ʾ���
}s_SystemType;

class CImageCapture :public QThread
{
	Q_OBJECT
public:
	CImageCapture(QThread *parent = NULL);
	~CImageCapture();

	void SetCameraType(e_CameraType type);
	void SetSystemType(s_SystemType SystemType);
	void SetCameraStatus(bool bOpen);
	void StopThread();
    bool SetCameraHandle(CMvCamera &camera, int index);
    void SetRunStatus(bool bStart);



private:
	CMvCamera m_MvCamera;
	QMutex m_Mutex;
	bool m_bStop;
	bool m_bOpen;
	bool m_bStartRun;
	int m_Index;
	unsigned char*  m_pGrabBuf;
	unsigned int nDataSize;
	e_CameraType m_Type;
	s_SystemType m_SystemType;


private:
	void run();
	Mat Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char * pData);
signals:
	//void SendCameraImage(Mat image, int index);
	void SendCameraImage(Mat image, int index);
    void SendAlgoImage(Mat OriginalImage, e_CameraType RenderImage, int index, bool bok);
	void SendImageToAlgo(Mat OriginalImage, e_CameraType type, int index, bool bok);
};