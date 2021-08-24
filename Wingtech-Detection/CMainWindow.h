#pragma once

#include <QtWidgets/QMainWindow>
#include <QLabel>
#include <QListView>
#include <QStandardItemModel>
#include "ui_CMainWindow.h"
#include "statisticswidget.h"
#include <opencv2/opencv.hpp>
#include "ParameterSetting.h"

using namespace std;
using namespace cv;





class CMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	CMainWindow(QWidget *parent = Q_NULLPTR);
	~CMainWindow();
private:
	//ÿ��ͼ��
	QVector<QImage> m_Camera1Images;
	QVector<QImage> m_Camera2Images;
	QVector<QImage> m_Camera3Images;
	QVector<QImage> m_Camera4Images;
	//�����ϸ
	QVector<QLabel *> m_Camera1Results;
	QVector<QLabel *> m_Camera2Results;
	QVector<QLabel *> m_Camera3Results;
	QVector<QLabel *> m_Camera4Results;

	//ÿ��ͼ���ս��
	bool m_Camera1Result;
	bool m_Camera2Result;
	bool m_Camera3Result;
	bool m_Camera4Result;

	//������ս��
	QLabel *m_TotalResult;
	StatisticsWidget *m_Statistics;
	QListView *m_ListView;
	QStandardItemModel *m_LogModel;

	//�����
	QMap<e_CameraType, bool> m_DetecionResult;

	//״̬��
	QLabel m_PLC;
	QLabel m_Camera1;
	QLabel m_Camera2;
	QLabel m_Camera3;
	QLabel m_Camera4;


	int m_Number;
	bool m_bStart;
private:
	void InitVariables();
	void InitStatusBar();
	void InitConnections();
	QImage MattoQImage(Mat image);
	void ProcessDetectionResult();
	void AddLog(QString log);

	//��ʼ�������ϸ
	void InitResultDetails(int ImageCounts);
	void RefreshResultDetails();

private:
	Ui::CMainWindowClass ui;
	CParameterSetting *m_Parameter;
protected:
	bool eventFilter(QObject *obj, QEvent *event);
private slots:
	void StartDection();
	void StopDection();
	void OpenSetting();

	//�������״̬
	void ReceiveCameraStatus(e_CameraType type, bool bOpen);
	//����PLC״̬
	void ReceivePLCStatus(bool bConnected);
	void ReceiveImage(Mat image, e_CameraType type, int index, bool bOK);
};
