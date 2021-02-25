#include "base/ProcMgr.h"
#include "hadaq/HldProcessor.h"
#include "hadaq/TdcProcessor.h"
#include "hadaq/TrbProcessor.h"

void first()
{

    base::ProcMgr::instance()->SetTriggeredAnalysis();
    base::ProcMgr::instance()->SetStoreKind(1);

   // base::ProcMgr::instance()->SetRawAnalysis(true);

   // this limits used for liner calibrations when nothing else is available
    hadaq::TdcMessage::SetFineLimits(31, 470);

   // default channel numbers and edges mask
    hadaq::TrbProcessor::SetDefaults(49, 0x3);

    hadaq::HldProcessor* hld = new hadaq::HldProcessor();

   // About time calibration - there are two possibilities
   // 1) automatic calibration after N hits in every enabled channel.
   //     Just use SetAutoCalibrations method for this
   // 2) generate calibration on base of provided data and than use it later statically for analysis
   //     Than one makes special run with SetWriteCalibrations() enabled.
   //     Later one reuse such calibrations enabling only LoadCalibrations() call

//    hadaq::TrbProcessor* trb3_1 = new hadaq::TrbProcessor(0x8000, hld);
//    trb3_1->SetHistFilling(4);
//    trb3_1->SetCrossProcess(true);
//    trb3_1->CreateTDC(0x6500,0xf303, 0xe003);
//    trb3_1->SetAutoCalibrations(50000);
//    trb3_1->SetWriteCalibrations("run1");


    hadaq::TrbProcessor* trb3_2 = new hadaq::TrbProcessor(0x8b10, hld);
    trb3_2->SetHistFilling(4);
    trb3_2->SetCrossProcess(true);
    trb3_2->CreateTDC(0x6440, 0x6441, 0x6442, 0x6443);
   // enable automatic calibration, specify required number of hits in each channel
    trb3_2->SetAutoCalibrations(50000);
   // calculate and write static calibration at the end of the run
    trb3_2->SetWriteCalibrations("run1");
   // load static calibration at the beginning of the run
   //trb3_2->LoadCalibrations("run1");

    hadaq::TrbProcessor* trb3_3 = new hadaq::TrbProcessor(0x8b11, hld);
    trb3_3->SetHistFilling(4);
    trb3_3->SetCrossProcess(true);
    trb3_3->CreateTDC(0x6450,0x6451, 0x6452, 0x6453);
   // enable automatic calibration, specify required number of hits in each channel
    trb3_3->SetAutoCalibrations(50000);
   // calculate and write static calibration at the end of the run
    trb3_3->SetWriteCalibrations("run1");
   // load static calibration at the beginning of the run
   //trb3_1->LoadCalibrations("run1");

    hadaq::TrbProcessor* trb3_4 = new hadaq::TrbProcessor(0x8b12, hld);
    trb3_4->SetHistFilling(4);
    trb3_4->SetCrossProcess(true);
    trb3_4->CreateTDC(0x6444,0x6445, 0x6454, 0x6455);
   // enable automatic calibration, specify required number of hits in each channel
    trb3_4->SetAutoCalibrations(50000);
   // calculate and write static calibration at the end of the run
    trb3_4->SetWriteCalibrations("run1");
   // load static calibration at the beginning of the run
   //trb3_1->LoadCalibrations("run1");

    hadaq::TrbProcessor* trb3_5 = new hadaq::TrbProcessor(0x8b13, hld);
    trb3_5->SetHistFilling(4);
    trb3_5->SetCrossProcess(true);
    trb3_5->CreateTDC(0x6460,0x6461, 0x6462, 0x6463);
   // enable automatic calibration, specify required number of hits in each channel
    trb3_5->SetAutoCalibrations(50000);
   // calculate and write static calibration at the end of the run
    trb3_5->SetWriteCalibrations("run1");
   // load static calibration at the beginning of the run
   //trb3_1->LoadCalibrations("run1");

    hadaq::TrbProcessor* trb3_6 = new hadaq::TrbProcessor(0x8b14, hld);
    trb3_6->SetHistFilling(4);
    trb3_6->SetCrossProcess(true);
    trb3_6->CreateTDC(0x6470,0x6471, 0x6472, 0x6473);
   // enable automatic calibration, specify required number of hits in each channel
    trb3_6->SetAutoCalibrations(50000);
   // calculate and write static calibration at the end of the run
    trb3_6->SetWriteCalibrations("run1");
   // load static calibration at the beginning of the run
   //trb3_1->LoadCalibrations("run1");

    hadaq::TrbProcessor* trb3_7 = new hadaq::TrbProcessor(0x8b15, hld);
    trb3_7->SetHistFilling(4);
    trb3_7->SetCrossProcess(true);
    trb3_7->CreateTDC(0x6464,0x6465, 0x6474, 0x6475);
   // enable automatic calibration, specify required number of hits in each channel
    trb3_7->SetAutoCalibrations(50000);
   // calculate and write static calibration at the end of the run
    trb3_7->SetWriteCalibrations("run1");
   // load static calibration at the beginning of the run
   //trb3_1->LoadCalibrations("run1");

    hadaq::TrbProcessor* trb3_8 = new hadaq::TrbProcessor(0x8b00, hld);
    trb3_8->SetHistFilling(4);
    trb3_8->SetCrossProcess(true);
    trb3_8->CreateTDC(0x6400,0x6401, 0x6402, 0x6403);
    trb3_8->SetAutoCalibrations(50000);
    trb3_8->SetWriteCalibrations("run1");

    hadaq::TrbProcessor* trb3_9 = new hadaq::TrbProcessor(0x8b01, hld);
    trb3_9->SetHistFilling(4);
    trb3_9->SetCrossProcess(true);
    trb3_9->CreateTDC(0x6410,0x6411, 0x6412, 0x6413);
    trb3_9->SetAutoCalibrations(50000);
    trb3_9->SetWriteCalibrations("run1");

    hadaq::TrbProcessor* trb3_10 = new hadaq::TrbProcessor(0x8b02, hld);
    trb3_10->SetHistFilling(4);
    trb3_10->SetCrossProcess(true);
    trb3_10->CreateTDC(0x6420,0x6421, 0x6422, 0x6423);
    trb3_10->SetAutoCalibrations(50000);
    trb3_10->SetWriteCalibrations("run1");

    hadaq::TrbProcessor* trb3_11 = new hadaq::TrbProcessor(0x8b03, hld);
    trb3_11->SetHistFilling(4);
    trb3_11->SetCrossProcess(true);
    trb3_11->CreateTDC(0x6430,0x6431, 0x6432, 0x6433);
    trb3_11->SetAutoCalibrations(50000);
    trb3_11->SetWriteCalibrations("run1");

    hadaq::TrbProcessor* trb3_12 = new hadaq::TrbProcessor(0x8c10, hld);
    trb3_12->SetHistFilling(4);
    trb3_12->SetCrossProcess(true);
    trb3_12->CreateTDC(0x6810,0x6811, 0x6812, 0x6813);
    trb3_12->SetAutoCalibrations(50000);
    trb3_12->SetWriteCalibrations("run1");


}



