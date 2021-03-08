
struct MNPlateInfoStruct
{
	int x, y, w, h;
	int plate_type;	// 0,1,2,3,
	char plate_code[64];
};


extern "C" PLATE_API int InitmnPlateReco(const char *region_cfg, const char* region_weights, const char *code_cfg, const char* code_weights);

extern "C" PLATE_API int ForwardmnPlateReco(const char *jpgfilename, MNPlateInfoStruct &plateinfo);

extern "C" PLATE_API int DisposemnPlateReco();


extern "C" PLATE_API int PLATE_InitmnReco(const char *region_cfg, const char* region_weights, const char *code_cfg, const char* code_weights);

extern "C" PLATE_API int PLATE_ForwardmnReco(const char *jpgfilename, MNPlateInfoStruct &plateinfo);
extern "C" PLATE_API int PLATE_ForwardmnRecoMat(unsigned char *matbuffer, int w, int h, MNPlateInfoStruct &plateinfo);
extern "C" PLATE_API int PLATE_DisposemnReco();


1\初始化

	if (bInited)	return;

	//int ret = InitmnPlateReco("obj_plate_region1226.cfg", "obj_plate_region1226_last.weights", "obj_plate_1225.cfg", "obj_plate_1225_last.weights");
	int ret = PLATE_InitmnReco("obj_plate_region1226.cfg", "obj_plate_region1226_last.weights", "obj_plate_1225.cfg", "obj_plate_1225_last.weights");
	if (ret >= 0) {
		AfxMessageBox(_T("init success."));
		bInited = true;
	}
	else
		AfxMessageBox(_T("初始化失败！"));
		
		
2、调用
	VideoCapture	cap;
	
	char buffer[256];
	memset(buffer, 0, 256);
	GetDlgItem(IDC_EDIT_URL)->GetWindowTextA(buffer, 256);

	int interval = m_comboInterval.GetCurSel() + 1;
	cap.open("G:\\huizhou.mp4");// buffer);
	int framenum = 0;
	if (1 || cap.isOpened())
	{
		Mat frame;

		while (cap.read(frame)) {

			framenum++;
			if (frame.empty())	continue;

			// reco every 5 frames
			if (framenum%interval == 1)
			{
				MNPlateInfoStruct	info;
				int cnt = PLATE_ForwardmnRecoMat((unsigned char *)frame.data, frame.cols, frame.rows, info);
				if (count < 7)
					continue;
				else{
					// display result;
				}
				return;			
			}

			imshow("", frame);

			waitKey(10);
		}
	}
	else {
		AfxMessageBox("open url failed.");
	}