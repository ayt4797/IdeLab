#ifndef _CAMERA_
#define _CAMERA_

void INIT_Camera(void);
void ms_delay(int del);
void plotline(uint16_t in_line[128]);
void bin_plotline(uint16_t in_line[128]);
void edge_detection(void);
void parsedata(void);
void camera_calibration(void);
void cameraUpsidedown(uint16_t line[128]);
void calibrate_center(void);
BOOLEAN isOffTrack(void);
#endif
