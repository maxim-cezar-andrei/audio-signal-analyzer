#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include <formatio.h>
#include <analysis.h>
#include "Interface.h"

#define SAMPLE_RATE		0
#define NPOINTS			1

static int panelHandle;
static int freqPanelHandle;
static int waveInfo[2]; //waveInfo[0] = sampleRate
//waveInfo[1] = number of elements
static double sampleRate = 0.0;
static int npoints = 0;
static double *waveData = NULL;
double *dataToPlot = NULL;
double* envelopeData = NULL;

static double min, max, mean, median, variance;
static int imin, imax;
static int zeros;
static int currentSecond=0;
static int totalSeconds=0;

static int currentIndex=0;

static int *histogram =0;
static double *cuante =0;
static int intervals=100;

int filterType;
int n_window;
double* filteredData=0;

char fileName[100];
int bitmapID;


int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "Interface.uir", PANEL_ACQ)) < 0)
		return -1;
	freqPanelHandle = LoadPanel (0, "Interface.uir", PANEL_FREQ);
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK OnAcqPanel (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			if (waveData != NULL)
			{
				free(waveData);
				waveData = NULL;
			}
			if (envelopeData != NULL)
			{
				free(envelopeData);
				envelopeData = NULL;
			}
			if (filteredData != NULL)
			{
				free(filteredData);
				filteredData = NULL;
			}
			QuitUserInterface(0);
			break;
	}
	return 0;
}


void PlotCurrentInterval(int panel)
{
	if (dataToPlot == NULL || sampleRate == 0) 
		return;

	int start_index = currentSecond * (int)sampleRate;
	int num_elements = (int)sampleRate;

	if (start_index + num_elements > npoints)
	{
		num_elements = npoints - start_index;
	}

	if (num_elements <= 0)
		return;

	DeleteGraphPlot(panel, PANEL_ACQ_GRAPH_RAW, -1, VAL_IMMEDIATE_DRAW);
	DeleteGraphPlot(panel, PANEL_ACQ_GRAPH_FILTERED, -1, VAL_IMMEDIATE_DRAW);

	PlotY(panel, PANEL_ACQ_GRAPH_RAW, &dataToPlot[start_index], num_elements, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);

	if (envelopeData != NULL)
	{
		PlotY(panel, PANEL_ACQ_GRAPH_RAW, &envelopeData[start_index], num_elements, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
	}

	if (filteredData != NULL)
	{
		SetAxisScalingMode (panel, PANEL_ACQ_GRAPH_FILTERED, VAL_LEFT_YAXIS, VAL_AUTOSCALE, 0.0, 0.0);
		SetAxisScalingMode (panel, PANEL_ACQ_GRAPH_FILTERED, VAL_BOTTOM_XAXIS, VAL_AUTOSCALE, 0.0, 0.0);
		PlotY(panel, PANEL_ACQ_GRAPH_FILTERED, &filteredData[start_index], num_elements, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_GREEN);
	}

	SetCtrlVal(panel, PANEL_ACQ_NUMERIC_START, currentSecond);
	SetCtrlVal(panel, PANEL_ACQ_NUMERIC_STOP, currentSecond + 1);

	//int bitmapID;
	//char fileName[100];

	sprintf(fileName, "Raw_Secunda_%d.jpg", currentSecond);

	GetCtrlDisplayBitmap(panel, PANEL_ACQ_GRAPH_RAW, 1, &bitmapID);

	SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);

	DiscardBitmap(bitmapID);

	if (filteredData != NULL)
	{
		sprintf(fileName, "Filtered_Secunda_%d.jpg", currentSecond);

		GetCtrlDisplayBitmap(panel, PANEL_ACQ_GRAPH_FILTERED, 1, &bitmapID);
		SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
		DiscardBitmap(bitmapID);
	}
}

int CVICALLBACK OnLoadWave (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{

	switch (event)
	{
		case EVENT_COMMIT:
			//executa script python pentru conversia unui fisierului .wav in .txt
			LaunchExecutable("python main.py");

			//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar)
			Delay(5);

			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("waveInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];

			totalSeconds = npoints / sampleRate;
			currentSecond = 0;
			SetCtrlVal(panel, PANEL_ACQ_NUMERIC_START, currentSecond);
			SetCtrlVal(panel, PANEL_ACQ_NUMERIC_STOP, currentSecond + 1);
			//PlotCurrentInterval(panel);

			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));

			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);

			envelopeData = (double *)calloc(npoints, sizeof(double));
			FileToArray("envelope.txt", envelopeData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			dataToPlot = waveData;
			
			/*double duration = (double)npoints / sampleRate;
			int pointsToPlot = npoints;
			if (duration > 10.0 && sampleRate > 0)
			{
				pointsToPlot = (int)(6.0 * sampleRate);
				dataToPlot = waveData;
				MessagePopup("Info", "Fisier > 10s. Se afiseaza primele 6 secunde.");
			}*/

			PlotCurrentInterval(panel);

			MaxMin1D (dataToPlot, npoints, &max, &imax, &min, &imin);

			Mean (dataToPlot, npoints, &mean);
			Median (dataToPlot, npoints, &median);
			Variance (dataToPlot, npoints, &mean, &variance);

			histogram = (int *) calloc(intervals, sizeof(double));
			cuante = (double *) calloc(intervals, sizeof(double));
			double *histDouble = (double *)calloc(intervals, sizeof(double));

			Histogram(dataToPlot, npoints, min, max, histogram, cuante, intervals);

			if (histDouble != NULL)
			{
				for (int i = 0; i < intervals; i++)
				{
					histDouble[i] = (double)histogram[i];
				}
				PlotXY(panelHandle, PANEL_ACQ_GRAPH_HIST, cuante, histDouble, intervals, VAL_DOUBLE, VAL_DOUBLE, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_BLUE);
				
				sprintf(fileName, "Histograma16.jpg"); 

                GetCtrlDisplayBitmap(panel, PANEL_ACQ_GRAPH_HIST, 1, &bitmapID);
                
                SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
                
                DiscardBitmap(bitmapID);
			}

			zeros = 0;
			for (int i = 1; i < npoints; i++)
			{
				if ((dataToPlot[i - 1] <= 0 && dataToPlot[i] > 0) || (dataToPlot[i - 1] >= 0 && dataToPlot[i] < 0))
					zeros++;
			}

			int n_samples = 256;

			if (npoints >= n_samples)
			{
				double mean_val = 0.0;
				double variance_val = 0.0;
				double moment3 = 0.0;
				double moment4 = 0.0;
				double skewness = 0.0;
				double kurtosis = 0.0;

				Variance(dataToPlot, n_samples, &mean_val, &variance_val);

				double std_dev = sqrt(variance_val);

				Moment(dataToPlot, n_samples, 3, &moment3);

				Moment(dataToPlot, n_samples, 4, &moment4);

				if (std_dev != 0)
				{
					skewness = moment3 / (std_dev * std_dev * std_dev);
					kurtosis = moment4 / (variance_val * variance_val);
				}

				SetCtrlVal(panel, PANEL_ACQ_NUMERIC_SKEW, skewness);
				SetCtrlVal(panel, PANEL_ACQ_NUMERIC_KURT, kurtosis);

				SetCtrlVal(panel, PANEL_ACQ_NUMERIC_MEAN, mean);
				SetCtrlVal(panel, PANEL_ACQ_NUMERIC_MIN, min);
				SetCtrlVal(panel, PANEL_ACQ_NUMERIC_MAX, max);
				SetCtrlVal(panel, PANEL_ACQ_NUMERIC_IMIN, imin);
				SetCtrlVal(panel, PANEL_ACQ_NUMERIC_IMAX, imax);
				SetCtrlVal(panel, PANEL_ACQ_NUMERIC_DISP, variance);
				SetCtrlVal(panel, PANEL_ACQ_NUMERIC_MEDIAN, median);
				SetCtrlVal(panel, PANEL_ACQ_NUMERIC_ZEROS, zeros);

			}
			break;
	}
	return 0;
}


int CVICALLBACK OnButtonApply (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if (dataToPlot == NULL || npoints == 0)
			{
				MessagePopup("Eroare", "Te rog încarca mai întâi un fisier (Load Wave)!");
				return 0;
			}

			filteredData = (double *)calloc(npoints, sizeof(double));

			if (filteredData == NULL)
				return -1;

			int deriv = 0;
			GetCtrlVal(panel, PANEL_ACQ_SWITCH_DERIV, &deriv);

			if(deriv == 1)
			{
				filteredData[0]=0;
				for (int i = 1; i < npoints; i++)
				{
					filteredData[i] = dataToPlot[i] - dataToPlot[i-1];
				}
			}
			else
			{
				double inputVal=0.0;
				GetCtrlVal(panel, PANEL_ACQ_NUMERIC_WINDOW, &inputVal);
				GetCtrlVal(panelHandle, PANEL_ACQ_RING_FILTER, &filterType);
				switch (filterType)
				{
					case 0: // MEDIERE
						int n_window=(int)inputVal;
						if (n_window <= 0)
						{
							MessagePopup("Atentie", "Dimensiunea ferestrei trebuie sa fie mai mare decat 0!");
							free(filteredData);
							return 0;
						}
						for (int i = 0; i < npoints; i++)
						{
							if (i < n_window - 1)
							{
								filteredData[i] = dataToPlot[i];
							}
							else
							{
								double sum = 0.0;
								for (int j = 0; j < n_window; j++)
								{
									sum += dataToPlot[i - j];
								}
								filteredData[i] = sum / n_window;
							}
						}
						break;

					case 1: // ELEMENT DE ORDINUL I
						double alpha=inputVal;
						if (alpha <= 0.0 || alpha >= 1.0)
						{
							MessagePopup("Eroare", "Coeficientul Alpha trebuie sa fie intre 0 si 1 (ex: 0.1, 0.05)!");
							free(filteredData);
							return 0;
						}

						filteredData[0] = dataToPlot[0];
						for (int i = 1; i < npoints; i++)
						{
							filteredData[i] = (1.0 - alpha) * filteredData[i-1] + alpha * dataToPlot[i];
						}
						break;
				}

			}

			PlotCurrentInterval(panel);

			break;
	}
	return 0;
}

int CVICALLBACK OnFilterChange (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int filterType;
			GetCtrlVal(panel, PANEL_ACQ_RING_FILTER, &filterType);

			if (filterType == 0)
			{

				SetCtrlAttribute(panel, PANEL_ACQ_NUMERIC_WINDOW, ATTR_LABEL_TEXT, "Dimensiune fereastra (n)");
				SetCtrlAttribute(panel, PANEL_ACQ_NUMERIC_WINDOW, ATTR_PRECISION, 0);
			}
			else if (filterType == 1)
			{
				SetCtrlAttribute(panel, PANEL_ACQ_NUMERIC_WINDOW, ATTR_LABEL_TEXT, "Coeficient Alpha (0 - 1)");
				SetCtrlAttribute(panel, PANEL_ACQ_NUMERIC_WINDOW, ATTR_PRECISION, 3);
			}
			break;
	}
	return 0;
}

int CVICALLBACK OnButtonPrev (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if (currentSecond > 0)
			{
				currentSecond--;
				PlotCurrentInterval(panel);
			}
			break;
	}
	return 0;
}

int CVICALLBACK OnButtonNext (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if (currentSecond < totalSeconds - 1)
			{
				currentSecond++;
				PlotCurrentInterval(panel);
			}
			break;
	}
	return 0;
}

int CVICALLBACK OnSwitchPanelCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	int switchState = 0;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, control, &switchState);

			if (panel == panelHandle)
			{
				SetCtrlVal(freqPanelHandle, PANEL_FREQ_SWITCH_CB, 1);

				DisplayPanel(freqPanelHandle);
				HidePanel(panelHandle);
			}
			else
			{
				SetCtrlVal(panelHandle, PANEL_ACQ_SWITCH_CB, 0);

				DisplayPanel(panelHandle);
				HidePanel(freqPanelHandle);
			}
			break;
	}
	return 0;
}

int CVICALLBACK OnTimer (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			int nr_puncte;
			int windowType=0;
			double frequencyPeak=0.0, powerPeak=0.0;

			GetCtrlVal(panel, PANEL_FREQ_RING_SPECTRU, &windowType);
			GetCtrlVal(panel, PANEL_FREQ_NUMERIC_PUNCTE, &nr_puncte);
			if(nr_puncte<32)
				return 0;

			int sourceSelection=0;
			GetCtrlVal(panel, PANEL_FREQ_BINARYSWITCH_SPECTRU, &sourceSelection);

			if(waveData==NULL || npoints==0)
				return 0;

			if(currentIndex+nr_puncte>npoints)
			{
				SetCtrlAttribute(panel, PANEL_FREQ_TIMER, ATTR_ENABLED, 0);
				return 0;
			}

			double *currentSegment=(double*)malloc(nr_puncte*sizeof(double));
			double *autoSpectrum=(double*)malloc((nr_puncte/2)*sizeof(double));
			double *convertedSpectrum=(double*)malloc((nr_puncte/2)*sizeof(double));
			WindowConst winConst;
			double df=0.0;
			double dt=1.0/sampleRate;
			double* sourceData=waveData;
			if (sourceSelection == 1)
			{
				if (filteredData != NULL)
				{
					sourceData = filteredData;
					SetCtrlAttribute(panel, PANEL_FREQ_GRAPH_SPECTRUM, ATTR_LABEL_TEXT, "Spectrul de putere (FILTRAT)");
				}
				else
				{
					SetCtrlAttribute(panel, PANEL_FREQ_GRAPH_SPECTRUM, ATTR_LABEL_TEXT, "Spectrul de putere (RAW - Lipsa date filtrate!)");
				}
			}
			else
			{
				SetCtrlAttribute(panel, PANEL_FREQ_GRAPH_SPECTRUM, ATTR_LABEL_TEXT, "Spectrul de putere (RAW)");
			}

			for(int i=0; i<nr_puncte; i++)
			{
				currentSegment[i]=sourceData[currentIndex+i];
			}

			ScaledWindowEx(currentSegment, nr_puncte, windowType, 0, &winConst);
			AutoPowerSpectrum(currentSegment, nr_puncte, dt, autoSpectrum, &df);
			PowerFrequencyEstimate(autoSpectrum, nr_puncte/2, -1.0, winConst, df, 7, &frequencyPeak, &powerPeak);
			SetCtrlVal(panel, PANEL_FREQ_NUMERIC_FREQ_PEAK, frequencyPeak);
			SetCtrlVal(panel, PANEL_FREQ_NUMERIC_POWER_PEAK, powerPeak);
			char unitString[32]="V";
			SpectrumUnitConversion(autoSpectrum, nr_puncte/2, 0, 0, 0, df, winConst, convertedSpectrum, unitString);
			DeleteGraphPlot(panel, PANEL_FREQ_GRAPH_SPECTRUM, -1, VAL_IMMEDIATE_DRAW);
			PlotWaveform(panel, PANEL_FREQ_GRAPH_SPECTRUM, convertedSpectrum, nr_puncte/2, VAL_DOUBLE, 1.0, 0.0, 0.0, df, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
			currentIndex+=nr_puncte;
			free(currentSegment);
			free(autoSpectrum);
			free(convertedSpectrum);
			break;
	}
	return 0;
}

int CVICALLBACK OnFreqPanel (int panel, int event, void *callbackData,
							 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK OnButtonSave (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int imghandle;
			char fileName[100];
			char timeBuff[100];
			double currSeconds;

			GetCurrentDateTime(&currSeconds);
			FormatDateTimeString(currSeconds, "%Y-%m-%d_Ora-%H-%M-%S", timeBuff, 100);
			sprintf(fileName, "Spectru_%s.jpg", timeBuff);

			GetCtrlDisplayBitmap(panel, PANEL_FREQ_GRAPH_SPECTRUM, 1, &imghandle);
			SaveBitmapToJPEGFile(imghandle, fileName, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(imghandle);
			MessagePopup("Succes", "Imaginea spectrului a fost salvata");
			break;
	}
	return 0;
}
