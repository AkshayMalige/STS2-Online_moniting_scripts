#include <stdio.h>

#include "TH1.h"

#include "TH2.h"

#include "TFile.h"

#include "TDirectory.h"

#include "TCanvas.h"

#include "base/EventProc.h"

#include "base/Event.h"

#include "base/ProcMgr.h"

#include "hadaq/TdcSubEvent.h"

#include "hadaq/definess.h"

#include "hadaq/HldProcessor.h"

#include "hadaq/TrbProcessor.h"

#include "hadaq/TdcProcessor.h"

#include <sstream>

#include <string>

#include <iostream>

#include <string>

#include <vector>

#include <time.h>

#include <fstream>

#include <algorithm>

#include <TError.h>

#include <TRint.h>

#include <TROOT.h>

#include <TSystem.h>

#include <TEnv.h>

#include <TCanvas.h>

#include <TView.h>

#include <TGeoManager.h>

#include <TGeoTrack.h>

#include <TVirtualGeoTrack.h>

#include <TLine.h>


#include <TFile.h>

#include <TMath.h>

#include <TH2F.h>

#include <TTree.h>

#include <TChain.h>

#include <TStopwatch.h>

#include <fstream>

#include <stdio.h>

#define NUMBER_OF_TDCS 45
#define CHANNELS_OFFSET 49

#define MAX_HITS 150
#define STRAWS 260
#define CHANNELS 2600
#define LAYERS 8
#define MODULES 56

using namespace std;

vector < string > & split(const string & s, char delim, vector < string > & elems) {

   stringstream ss(s);

   string item;

   while (getline(ss, item, delim)) {

      elems.push_back(item);

   }

   return elems;

}

vector < string > split(const string & s, char delim) {

   vector < string > elems;

   split(s, delim, elems);

   return elems;

}

struct detLoc {
   detLoc() {};

   detLoc(int p_station, int p_layer, int p_straw) {
      layer = p_layer;
      station = p_station;
      straw = p_straw;
   }

   int layer;
   int station;
   int straw;
};

class Detector {
   public:
      map < int, detLoc > detMap;

   Detector();

   detLoc GetDetectorLocFromTDCChannel(int channel);

};

class LPetProcessor: public base::EventProc {

   private: int internalEventCtr;

   public:

      Detector * det;

   vector < hadaq::TdcSubEvent * > tdcs;

   TH1F * h_Tot;
   TH1F * h_driftTime;
   TH2F * h_driftTimeVsTOT;
   TH1F * h_leadTime;
   TH1F * h_trailTime;
   TH2F * h_TotVsChannel;
   TH2F * h_driftTimeVsChannel;
   TH2F * h_leadTimeVsChannel;
   TH2F * h_channelVsHits;
   TH1F * h_channelMult;
   TH1F * h_strawMult;
   TH1F * h_layerMultiplicity;
   TH1F * h_planeMultiplicity;
   TH1F * h_hitMultiplicity;
   TH1F * h_sts1_peff;
   TH1F * h_sts2_peff;
   TH1F * h_sts1_eff_straws;
   TH1F * h_sts2_eff_straws;
   TH1F * h_rpc_dt;
	TH1F * h_sts2_dt;
   TH1F * h_dummy_dt;
   TH1F * h_lt_diff;

   TH1F * h_rpc_lt;

   TH2F * h_sts1_eff_Lstraws;
   TH2F * h_sts2_eff_Lstraws;

   TH2D * h_FT_Geo;
   TH2D * h_FT_Geo2;

   TH1F * h_sts_entries;
   //TH2F* h_geo;

   TH1F * h_Layer_Tot[LAYERS];
   TH1F * h_Layer_Straw[LAYERS];
   TH1F * h_Layer_driftTime[LAYERS];
   //TH2F* h_Layer_driftTimeVsTOT[LAYERS]; 
   TH1F * h_Layer_leadTime[LAYERS];
   TH1F * h_Layer_trailTime[LAYERS];
   //TH2F* h_Layer_TotVsChannel[LAYERS];
   //TH2F* h_Layer_TotVsStraw[LAYERS];
   //TH2F* h_Layer_driftTimeVsChannel[LAYERS];
   //TH2F* h_Layer_leadTimeVsChannel[LAYERS];

   TH1F * h_Module_Tot[MODULES];
   TH1F * h_Module_driftTime[MODULES];
   //TH2F* h_Module_driftTimeVsTOT[MODULES]; 
   TH1F * h_Module_leadTime[MODULES];
   TH1F * h_Module_trailTime[MODULES];
   //TH2F* h_Module_TotVsChannel[MODULES];
   //TH2F* h_Module_TotVsStraw[MODULES];
   //TH2F* h_Module_driftTimeVsChannel[MODULES];
   //TH2F* h_Module_leadTimeVsChannel[MODULES];

   TH1F * h_refTimeTRB1;
   TH1F * h_refTimeTRB2;
   TH1F * h_refTimeTRB3;
   TH1F * h_refTimeTRB4;
   TH1F * h_refTimeTRB5;
   TH1F * h_refTimeTRB6;
   TH1F * h_refTimeTRB7;
   TH1F * h_refTimeTDC[NUMBER_OF_TDCS];

   LPetProcessor(): base::EventProc("LPET") {

      det = new Detector();

      std::map < UInt_t, int > stt_channel_offsets;

      stt_channel_offsets[6440] = 0 * 49;
      stt_channel_offsets[6441] = 1 * 49;
      stt_channel_offsets[6442] = 2 * 49;
      stt_channel_offsets[6443] = 3 * 49;
      stt_channel_offsets[6444] = 4 * 49;
      stt_channel_offsets[6445] = 5 * 49;
      stt_channel_offsets[6450] = 6 * 49;
      stt_channel_offsets[6451] = 7 * 49;
      stt_channel_offsets[6452] = 8 * 49;
      stt_channel_offsets[6453] = 9 * 49;
      stt_channel_offsets[6454] = 10 * 49;
      stt_channel_offsets[6455] = 11 * 49;
      stt_channel_offsets[6460] = 12 * 49;
      stt_channel_offsets[6461] = 13 * 49;
      stt_channel_offsets[6462] = 14 * 49;
      stt_channel_offsets[6463] = 15 * 49;
      stt_channel_offsets[6464] = 16 * 49;
      stt_channel_offsets[6465] = 17 * 49;
      stt_channel_offsets[6470] = 18 * 49;
      stt_channel_offsets[6471] = 19 * 49;
      stt_channel_offsets[6472] = 20 * 49;
      stt_channel_offsets[6473] = 21 * 49;
      stt_channel_offsets[6474] = 22 * 49;
      stt_channel_offsets[6475] = 23 * 49;
      stt_channel_offsets[6500] = 24 * 49;

      stt_channel_offsets[6400] = 25 * 49;
      stt_channel_offsets[6401] = 26 * 49;
      stt_channel_offsets[6402] = 27 * 49;
      stt_channel_offsets[6403] = 28 * 49;
      stt_channel_offsets[6410] = 29 * 49;
      stt_channel_offsets[6411] = 30 * 49;
      stt_channel_offsets[6412] = 31 * 49;
      stt_channel_offsets[6413] = 32 * 49;
      stt_channel_offsets[6420] = 33 * 49;
      stt_channel_offsets[6421] = 34 * 49;
      stt_channel_offsets[6422] = 35 * 49;
      stt_channel_offsets[6423] = 36 * 49;
      stt_channel_offsets[6430] = 37 * 49;
      stt_channel_offsets[6431] = 38 * 49;
      stt_channel_offsets[6432] = 39 * 49;
      stt_channel_offsets[6433] = 40 * 49;

      stt_channel_offsets[6810] = 41 * 49;
      stt_channel_offsets[6811] = 42 * 49;
      stt_channel_offsets[6812] = 43 * 49;
      stt_channel_offsets[6813] = 44 * 49;

      //     fstream infile("/home/pandastraws/go4/new_release/trb3/sts2test/sts2_map.txt");
      fstream infile("./sts_map.txt");
      string line;

      stringstream ss;

      if (!infile) {
         cout << "Error reading file" << endl;
         exit(0);
      }

      while (getline(infile, line)) {

         vector < string > elems = split(line, '\t');
         vector < int > int_elems;
         for (int a = 0; a < elems.size(); a++) {
            int num = atoi(elems.at(a).c_str());
            int_elems.push_back(num);
         }

         det -> detMap[stt_channel_offsets[int_elems.at(0)] + int_elems.at(1)] = detLoc(int_elems.at(2), int_elems.at(3), int_elems.at(4));

      }

      internalEventCtr = 0;

      tdcs = vector < hadaq::TdcSubEvent * > ();
      h_Tot = new TH1F("TOT", "TOT;Time Over Threshold [ns]", 1000, 0, 1000);
      h_driftTime = new TH1F("DriftTime", "DriftTime; Drift Time", 5000, -5000, 5000);
      h_driftTimeVsTOT = new TH2F("DriftTime_vs_TOT", "DriftTime_vs_TOT;Drift Time;Time Over Threshold", 600, -300, 300, 650, 0, 650);
      h_leadTime = new TH1F("leadTime", "leadTime;Lead time", 10000, -10000, 10000);
      h_lt_diff = new TH1F("h_lt_diff", "h_lt_diff;Lead time diff", 10000, -10000, 10000);
      h_trailTime = new TH1F("Trail_Time", "Trail_Time; Trail time", 10000, -10000, 10000);
      h_TotVsChannel = new TH2F("TOT_vs_Channel", "TOT_vs_Channel;Time over threshold [ns];TDC Channel No", 600, 0, 600, CHANNELS, 0, CHANNELS);
      h_driftTimeVsChannel = new TH2F("DriftTime_vs_Channel", "Drift_Time_vs_Channel;Drift Time;TDC Channel", 500, 0, 500, CHANNELS, 0, CHANNELS);
      h_leadTimeVsChannel = new TH2F("LeadTime_vs_Channel", "LeadTime_vs_Channel;Lead time;TDC Channel No", 10000, -10000, 10000, CHANNELS, 0, CHANNELS);
      h_channelMult = new TH1F("Channel_Mult", "channelMult;Channel No", CHANNELS, 0, CHANNELS);
      h_strawMult = new TH1F("Straw_Mult", "strawMult;Straw No", STRAWS * 9, 0, STRAWS * 9);
      h_channelVsHits = new TH2F("Channels_vs_Hits", "Channels_vs_Hits;channel no;No.hits/event", CHANNELS, 0, CHANNELS, 40, -10, 30);
      h_layerMultiplicity = new TH1F("Layer_Mult", "Layer;Hits", 10, 0, 10);
      h_planeMultiplicity = new TH1F("Plane_Multiplicity", "Plane;Hits", 20, 0, 20);
      h_hitMultiplicity = new TH1F("Hit_Multiplicity", "Hits;Hits", 100, 0, 100);
      h_sts1_peff = new TH1F("h_sts1_peff", "No. of Planes;No. of Planes", 10, 0, 10);
      h_sts2_peff = new TH1F("h_sts2_peff", "No. of Planes;No. of Planes", 10, 0, 10);
      h_sts1_eff_straws = new TH1F("h_sts1_eff_straws", "Straw multiplicity ;Straw multiplicity", 400, 1, 401);
      h_sts2_eff_straws = new TH1F("h_sts2_eff_straws", "Straw multiplicity;Straw multiplicity", 600, 1, 601);

      h_sts1_eff_Lstraws = new TH2F("h_sts1_eff_Lstraws", "Straw multiplicity ;Layer;Straw multiplicity", 6, 0, 6, 50, 0, 50);
      h_sts2_eff_Lstraws = new TH2F("h_sts2_eff_Lstraws", "Straw multiplicity;Layer;Straw multiplicity", 6, 0, 6, 50, 0, 50);

      h_sts_entries = new TH1F("h_sts_entries", "h_sts_entries;h_sts_entries", 5, 0, 5);

      h_rpc_lt = new TH1F("h_rpc_lt", "LT", 3000, -500, 2500);
      h_rpc_dt = new TH1F("h_rpc_dt", "dt", 2000, -1000, 1000);
      h_sts2_dt = new TH1F("h_sts2_dt", "dt [ ns ]", 2000, -1000, 1000);
      h_dummy_dt = new TH1F("h_dummy_dt", "dt", 2000, -1000, 1000);

      h_FT_Geo = new TH2D("h_FT_Geo", "h_FT_Geo;Straw No;Sub Layer no", 522, -1, 260, 20, 0, 10);
      h_FT_Geo2 = new TH2D("h_FT_Geo2", "h_FT_Geo2;Straw No;check Sub Layer no", 522, -1, 260, 20, 0, 10);
      //h_geo = new TH2F("geo", "geo", 522, 0, 260, 10, 0, 5);

      for (int i = 0; i < LAYERS; i++) {
         if (i < 4) {
            h_Layer_Tot[i] = new TH1F(Form("STS2_Layer_%d_TOT", i + 1), Form("STS2_Layer_%d_TOT;Time Over Threshold [ns]", i + 1), 1000, 0, 1000);
            h_Layer_driftTime[i] = new TH1F(Form("STS2_Layer_%d_DriftTime", i + 1), Form("STS2_Layer_%d_DriftTime;Drift Time [ns]", i + 1), 5000, -5000, 5000);
            //h_Layer_driftTimeVsTOT[i] = new TH2F(Form("STS2_Layer_%d_driftTimeVsTOT", i+1) , Form("STS2_Layer_%d_driftTimeVsTOT;Drift Time [ns];Time Over Threshold [ns]", i+1), 500, 0, 500, CHANNELS, 0, CHANNELS);
            h_Layer_leadTime[i] = new TH1F(Form("STS2_Layer_%d_leadTime", i + 1), Form("STS2_Layer_%d_leadTime;leadTime", i + 1), 10000, -10000, 10000);
            h_Layer_trailTime[i] = new TH1F(Form("STS2_Layer_%d_trailTime", i + 1), Form("STS2_Layer_%d_trailTime;trailTime", i + 1), 10000, -10000, 10000);
            //h_Layer_TotVsChannel[i] = new TH2F(Form("STS2_Layer_%d_TotVsChannel", i+1) , Form("STS2_Layer_%d_TotVsChannel;Time Over Threshold [ns];Channel No", i+1), 600, 0, 600, CHANNELS, 0, CHANNELS);
            //h_Layer_driftTimeVsChannel[i] = new TH2F(Form("STS2_Layer_%d_driftTimeVsChannel", i+1) , Form("STS2_Layer_%d_driftTimeVsChannel;Drift Time [ns];Straw No", i+1), 500, 0, 500, CHANNELS, 0, CHANNELS);
            //h_Layer_leadTimeVsChannel[i] = new TH2F(Form("STS2_Layer_%d_leadTimeVsChannel", i+1) , Form("STS2_Layer_%d_leadTimeVsChannel;leadTime;Straw No",i+1), 10000, -10000, 10000, CHANNELS, 0, CHANNELS);
            //h_Layer_TotVsStraw[i] = new TH2F(Form("STS2_Layer_%d_TotVsStraw", i+1) , Form("STS2_Layer_%d_TotVsStraw;Time Over Threshold [ns];Straw No", i+1), 600, 0, 600, 260, 0, 260);
         } else {
            h_Layer_Tot[i] = new TH1F(Form("STS1_Layer_%d_TOT", i - 3), Form("STS1_Layer_%d_TOT;Time Over Threshold [ns]", i - 3), 1000, 0, 1000);
            h_Layer_driftTime[i] = new TH1F(Form("STS1_Layer_%d_DriftTime", i - 3), Form("STS1_Layer_%d_DriftTime;Drift Time [ns]", i - 3), 5000, -5000, 5000);
            //h_Layer_driftTimeVsTOT[i] = new TH2F(Form("STS1_Layer_%d_driftTimeVsTOT", i-3) , Form("STS1_Layer_%d_driftTimeVsTOT;Drift Time [ns];Time Over Threshold [ns]", i-3), 500, 0, 500, CHANNELS, 0, CHANNELS);
            h_Layer_leadTime[i] = new TH1F(Form("STS1_Layer_%d_leadTime", i - 3), Form("STS1_Layer_%d_leadTime;leadTime", i - 3), 10000, -10000, 10000);
            h_Layer_trailTime[i] = new TH1F(Form("STS1_Layer_%d_trailTime", i - 3), Form("STS1_Layer_%d_trailTime;trailTime", i - 3), 10000, -10000, 10000);
            //h_Layer_TotVsChannel[i] = new TH2F(Form("STS1_Layer_%d_TotVsChannel", i-3) , Form("STS1_Layer_%d_TotVsChannel;Time Over Threshold [ns];Channel No", i-3), 600, 0, 600, CHANNELS, 0, CHANNELS);
            //h_Layer_driftTimeVsChannel[i] = new TH2F(Form("STS1_Layer_%d_driftTimeVsChannel", i-3) , Form("STS1_Layer_%d_driftTimeVsChannel;Drift Time [ns];Straw No", i-3), 500, 0, 500, CHANNELS, 0, CHANNELS);
            //h_Layer_leadTimeVsChannel[i] = new TH2F(Form("STS1_Layer_%d_leadTimeVsChannel", i-3) , Form("STS1_Layer_%d_leadTimeVsChannel;leadTime;Straw No",i-3), 10000, -10000, 10000, CHANNELS, 0, CHANNELS);
            //h_Layer_TotVsStraw[i] = new TH2F(Form("STS1_Layer_%d_TotVsStraw", i-3) , Form("STS1_Layer_%d_TotVsStraw;Time Over Threshold [ns];Straw No", i-3), 600, 0, 600, 260, 0, 260);
         }

      }

      for (int i = 0; i < MODULES; i++) {
         if (i < 32) {
            h_Module_Tot[i] = new TH1F(Form("STS2_Module_%d_TOT", i + 1), Form("STS2_Module_%d_TOT;Time Over Threshold [ns]", i + 1), 1000, 0, 1000);
            h_Module_driftTime[i] = new TH1F(Form("STS2_Module_%d_DriftTime", i + 1), Form("STS2_Module_%d_DriftTime;Drift Time [ns]", i + 1), 5000, -5000, 5000);
            //h_Module_driftTimeVsTOT[i] = new TH2F(Form("STS2_Module_%d_driftTimeVsTOT", i+1) , Form("STS2_Module_%d_driftTimeVsTOT;Drift Time [ns];Time Over Threshold [ns]", i+1), 500, 0, 500, 1050, 0, 1050);
            h_Module_leadTime[i] = new TH1F(Form("STS2_Module_%d_leadTime", i + 1), Form("STS2_Module_%d_leadTime;leadTime", i + 1), 10000, -10000, 10000);
            h_Module_trailTime[i] = new TH1F(Form("STS2_Module_%d_trailTime", i + 1), Form("STS2_Module_%d_trailTime;trailTime", i + 1), 10000, -10000, 10000);
            //h_Module_TotVsChannel[i] = new TH2F(Form("STS2_Module_%d_TotVsChannel", i+1) , Form("STS2_Module_%d_TotVsChannel;Time Over Threshold [ns];TDC Ch No", i+1), 600, 0, 600, 33, 0, 33);
            //h_Module_TotVsStraw[i] = new TH2F(Form("STS2_Module_%d_TotVsStraw", i+1) , Form("STS2_Module_%d_TotVsStraw;Time Over Threshold [ns];Straw No", i+1), 600, 0, 600, 33, 0, 33);
            //h_Module_driftTimeVsChannel[i] = new TH2F(Form("STS2_Module_%d_driftTimeVsChannel", i+1) , Form("STS2_Module_%d_driftTimeVsChannel;Drift Time [ns];Straw No", i+1), 500, 0, 500, 1050, 0, 1050);
            //h_Module_leadTimeVsChannel[i] = new TH2F(Form("STS2_Module_%d_leadTimeVsChannel", i+1) , Form("STS2_Module_%d_leadTimeVsChannel;leadTime;Straw No",i+1), 10000, -10000, 10000, 33, 0, 33);
         } else {
            h_Module_Tot[i] = new TH1F(Form("STS1_Module_%d_TOT", i - 31), Form("STS1_Module_%d_TOT;Time Over Threshold [ns]", i - 31), 1000, 0, 1000);
            h_Module_driftTime[i] = new TH1F(Form("STS1_Module_%d_DriftTime", i - 31), Form("STS1_Module_%d_DriftTime;Drift Time [ns]", i - 31), 5000, -5000, 5000);
            //h_Module_driftTimeVsTOT[i] = new TH2F(Form("Module_%d_driftTimeVsTOT", i-31) , Form("STS1_Module_%d_driftTimeVsTOT;Drift Time [ns];Time Over Threshold [ns]", i-31), 500, 0, 500, 1050, 0, 1050);
            h_Module_leadTime[i] = new TH1F(Form("STS1_Module_%d_leadTime", i - 31), Form("STS1_Module_%d_leadTime;leadTime", i - 31), 10000, -10000, 10000);
            h_Module_trailTime[i] = new TH1F(Form("STS1_Module_%d_trailTime", i - 31), Form("STS1_Module_%d_trailTime;trailTime", i - 31), 10000, -10000, 10000);
            //h_Module_TotVsChannel[i] = new TH2F(Form("STS1_Module_%d_TotVsChannel", i-31) , Form("STS1_Module_%d_TotVsChannel;Time Over Threshold [ns];TDC Ch No", i-31), 600, 0, 600, 33, 0, 33);
            //h_Module_TotVsStraw[i] = new TH2F(Form("STS1_Module_%d_TotVsStraw", i-31) , Form("STS1_Module_%d_TotVsStraw;Time Over Threshold [ns];Straw No", i-31), 600, 0, 600, 33, 0, 33);
            //h_Module_driftTimeVsChannel[i] = new TH2F(Form("STS1_Module_%d_driftTimeVsChannel", i-31) , Form("STS1_Module_%d_driftTimeVsChannel;Drift Time [ns];Straw No", i-31), 500, 0, 500, 1050, 0, 1050);
            //h_Module_leadTimeVsChannel[i] = new TH2F(Form("STS1_Module_%d_leadTimeVsChannel", i-31) , Form("STS1_Module_%d_leadTimeVsChannel;leadTime;Straw No",i-31), 10000, -10000, 10000, 33, 0, 33);
         }
      }

      h_refTimeTRB1 = new TH1F("Ref_Time_TRB1", "Ref_Time_TRB1", 500, -25, 25);
      h_refTimeTRB2 = new TH1F("Ref_Time_TRB2", "Ref_Time_TRB2", 500, -25, 25);
      h_refTimeTRB3 = new TH1F("Ref_Time_TRB3", "Ref_Time_TRB3", 500, -25, 25);
      h_refTimeTRB4 = new TH1F("Ref_Time_TRB4", "Ref_Time_TRB4", 500, -25, 25);
      h_refTimeTRB5 = new TH1F("Ref_Time_TRB5", "Ref_Time_TRB5", 500, -25, 25);
      h_refTimeTRB6 = new TH1F("Ref_Time_TRB6", "Ref_Time_TRB6", 500, -25, 25);
      h_refTimeTRB7 = new TH1F("Ref_Time_TRB7", "Ref_Time_TRB7", 500, -25, 25);

      for (int i = 0; i < NUMBER_OF_TDCS; i++) {
         h_refTimeTDC[i] = new TH1F(Form("Ref_Time_TDC%d", i), Form("Ref_Time_TDC%d", i), 500, -2500, 2500);
      }

      RegisterObject(h_FT_Geo, "Geometry");
      RegisterObject(h_FT_Geo2, "Geometry");
      //RegisterObject(h_geo, "Geometry");

      RegisterObject(h_Tot, "GeneralInfo");
      //	RegisterObject(h_driftTime, "GeneralInfo");
      RegisterObject(h_driftTimeVsTOT, "GeneralInfo");
      RegisterObject(h_leadTime, "GeneralInfo");
      RegisterObject(h_trailTime, "GeneralInfo");
      RegisterObject(h_TotVsChannel, "GeneralInfo");
      RegisterObject(h_lt_diff, "GeneralInfo");
      //	RegisterObject(h_driftTimeVsChannel, "GeneralInfo");
      RegisterObject(h_leadTimeVsChannel, "GeneralInfo");
      RegisterObject(h_channelMult, "GeneralInfo");
      RegisterObject(h_strawMult, "GeneralInfo");
      RegisterObject(h_channelVsHits, "GeneralInfo");
      RegisterObject(h_layerMultiplicity, "GeneralInfo");
      RegisterObject(h_planeMultiplicity, "GeneralInfo");
      RegisterObject(h_hitMultiplicity, "GeneralInfo");
      RegisterObject(h_sts1_peff, "GeneralInfo");
      RegisterObject(h_sts2_peff, "GeneralInfo");
      //RegisterObject(h_sts1_eff_straws, "GeneralInfo");
      //RegisterObject(h_sts2_eff_straws, "GeneralInfo");
      RegisterObject(h_sts1_eff_Lstraws, "GeneralInfo");
      RegisterObject(h_sts2_eff_Lstraws, "GeneralInfo");
      RegisterObject(h_rpc_lt, "GeneralInfo");
      RegisterObject(h_rpc_dt, "GeneralInfo");
      RegisterObject(h_sts2_dt, "GeneralInfo");
      RegisterObject(h_sts_entries, "GeneralInfo");

      for (int i = 0; i < LAYERS; i++) {
         RegisterObject(h_Layer_Tot[i], "LayerInfo");
         //  RegisterObject(h_Layer_driftTime[i], "LayerInfo");
         //  RegisterObject(h_Layer_driftTimeVsTOT[i], "LayerInfo");
         RegisterObject(h_Layer_leadTime[i], "LayerInfo");
         RegisterObject(h_Layer_trailTime[i], "LayerInfo");
         // RegisterObject(h_Layer_TotVsChannel[i], "LayerInfo");
         // RegisterObject(h_Layer_driftTimeVsChannel[i], "LayerInfo");
         //   RegisterObject(h_Layer_leadTimeVsChannel[i], "LayerInfo");
         //RegisterObject(h_Layer_TotVsStraw[i], "LayerInfo");
      }

      for (int i = 0; i < MODULES; i++) {
         RegisterObject(h_Module_Tot[i], "ModuleInfo");
         // RegisterObject(h_Module_driftTime[i], "ModuleInfo");
         //RegisterObject(h_Module_driftTimeVsTOT[i], "ModuleInfo");
         RegisterObject(h_Module_leadTime[i], "ModuleInfo");
         RegisterObject(h_Module_trailTime[i], "ModuleInfo");
         // RegisterObject(h_Module_TotVsChannel[i], "ModuleInfo");
         // RegisterObject(h_Module_TotVsStraw[i], "ModuleInfo");
         // RegisterObject(h_Module_driftTimeVsChannel[i], "ModuleInfo");
         //    RegisterObject(h_Module_leadTimeVsChannel[i], "ModuleInfo");
      }

      for (int i = 0; i < NUMBER_OF_TDCS; i++) {
         RegisterObject(h_refTimeTDC[i], "tdcs");
      }

      RegisterObject(h_refTimeTRB1, "tdcs");
      RegisterObject(h_refTimeTRB2, "tdcs");
      RegisterObject(h_refTimeTRB3, "tdcs");
      RegisterObject(h_refTimeTRB4, "tdcs");
      RegisterObject(h_refTimeTRB5, "tdcs");
      RegisterObject(h_refTimeTRB6, "tdcs");
      RegisterObject(h_refTimeTRB7, "tdcs");

   }

   virtual bool Process(base::Event * ev) {

      internalEventCtr++;

      //cout<<"Event: "<<internalEventCtr<<endl;

      if (internalEventCtr % 10000 == 0) cout << "Event: " << internalEventCtr << endl;

      tdcs.clear();

      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6440")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6441")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6442")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6443")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6444")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6445")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6450")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6451")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6452")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6453")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6454")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6455")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6460")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6461")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6462")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6463")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6464")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6465")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6470")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6471")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6472")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6473")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6474")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6475")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6500")));

      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6400")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6401")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6402")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6403")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6410")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6411")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6412")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6413")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6420")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6421")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6422")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6423")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6430")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6431")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6432")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6433")));

      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6810")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6811")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6812")));
      tdcs.push_back(dynamic_cast < hadaq::TdcSubEvent * > (ev -> GetSubEvent("TDC_6813")));

      // reference times
      double refTime[NUMBER_OF_TDCS];

      double leadTimes[CHANNELS][MAX_HITS];
      double trailTimes[CHANNELS][MAX_HITS];
      int leadTimesNum[CHANNELS];
      int trailTimesNum[CHANNELS];
      double Tot[CHANNELS][MAX_HITS];
      int TotNum[CHANNELS];
      int TotStraw[LAYERS][STRAWS];
      int TotMod[MODULES];
      int TotLay[LAYERS];
      int TotPlane[LAYERS * 2];

      detLoc loc;
      double hit_time = 0;
      double scint_time = 0;
      double scint_ref = 0;
      double ref_diff = 0;
      double drifttime = 1;
      bool scint_hit = false;

      for (int i = 0; i < CHANNELS; i++) {
         leadTimesNum[i] = 0;
         trailTimesNum[i] = 0;
         TotNum[i] = 0;
      }
      for (int i = 0; i < LAYERS * 2; i++) {
         TotPlane[i] = 0;
      }

      for (int i = 0; i < CHANNELS; i++) {
         leadTimesNum[i] = 0;
         TotNum[i] = 0;
      }

      int hitMultOnLayer[LAYERS + 1];
      int dtHitsMult[LAYERS + 1][CHANNELS + 1];

      for (int l = 0; l <= LAYERS; l++) {
         hitMultOnLayer[l] = 0;
         TotLay[l] = 0;
         for (int m = 0; m <= CHANNELS; m++) {
            dtHitsMult[l][m] = 0;
         }

         for (int i = 0; i < STRAWS; i++) {
            TotStraw[l][i] = 0;
         }
      }

      // gather hit times
      int tdc_ptr = 0;
      vector < hadaq::TdcSubEvent * > ::iterator tdcs_it;
      for (tdcs_it = tdcs.begin(); tdcs_it != tdcs.end(); tdcs_it++) {
         //cout<<"TDC: "<<tdc_ptr<<endl;
         tdc_ptr++;
         if (( * tdcs_it) == 0) continue;
         for (unsigned cnt = 0; cnt < ( * tdcs_it) -> Size(); cnt++) {
            const hadaq::TdcMessageExt & ext = ( * tdcs_it) -> msg(cnt);

            int local_ch = ext.msg().getHitChannel() + CHANNELS_OFFSET * (tdc_ptr - 1);

            if (ext.msg().isHitRisingEdge() == true) {
               leadTimes[local_ch][leadTimesNum[local_ch]] = ext.GetGlobalTime() * 1e9;
               leadTimesNum[local_ch]++;

            } else if (ext.msg().isHitRisingEdge() == false) {
               trailTimes[local_ch][trailTimesNum[local_ch]] = ext.GetGlobalTime() * 1e9;
               trailTimesNum[local_ch]++;
            }

            if (ext.msg().isHitRisingEdge() == true) {
               if (ext.msg().getHitChannel() == 0) {
                  refTime[tdc_ptr - 1] = ext.GetGlobalTime() * 1e9;
               }
               h_channelMult -> Fill(ext.msg().getHitChannel() + CHANNELS_OFFSET * (tdc_ptr - 1));

               //  h_leadTimeVsChannel->Fill((ext.GetGlobalTime() * 1e9), ext.msg().getHitChannel() + CHANNELS_OFFSET * (tdc_ptr - 1));

               loc = det -> GetDetectorLocFromTDCChannel(ext.msg().getHitChannel() + CHANNELS_OFFSET * (tdc_ptr - 1));
               int tdc_ad = ext.msg().getHitChannel() + CHANNELS_OFFSET * (tdc_ptr - 1);

               //  h_layerChannelVsLeadTime[loc.layer - 1]->Fill((ext.GetGlobalTime() * 1e9),  loc.straw);  
               //printf("%lf \t %lf\n",leadTimes[1176][leadTimesNum[1176]],leadTimes[1177][leadTimesNum[1177]])  ;  
               //if(tdc_ptr >41) printf("%i,  %i  , %lf \n",tdc_ptr,local_ch,ext.GetGlobalTime() * 1e9);               

            }

         }
      }

      h_refTimeTRB1 -> Fill(refTime[0] - refTime[1]);
      h_refTimeTRB1 -> Fill(refTime[0] - refTime[2]);
      h_refTimeTRB1 -> Fill(refTime[0] - refTime[3]);

      h_refTimeTRB2 -> Fill(refTime[4] - refTime[5]);
      h_refTimeTRB2 -> Fill(refTime[4] - refTime[6]);
      h_refTimeTRB2 -> Fill(refTime[4] - refTime[7]);

      h_refTimeTRB3 -> Fill(refTime[8] - refTime[9]);
      h_refTimeTRB3 -> Fill(refTime[8] - refTime[10]);
      h_refTimeTRB3 -> Fill(refTime[8] - refTime[11]);

      h_refTimeTRB4 -> Fill(refTime[12] - refTime[13]);
      h_refTimeTRB4 -> Fill(refTime[12] - refTime[14]);
      h_refTimeTRB4 -> Fill(refTime[12] - refTime[15]);

      h_refTimeTRB5 -> Fill(refTime[16] - refTime[17]);
      h_refTimeTRB5 -> Fill(refTime[16] - refTime[18]);
      h_refTimeTRB5 -> Fill(refTime[16] - refTime[19]);

      h_refTimeTRB6 -> Fill(refTime[20] - refTime[21]);
      h_refTimeTRB6 -> Fill(refTime[20] - refTime[22]);
      h_refTimeTRB6 -> Fill(refTime[20] - refTime[23]);

      h_refTimeTDC[0] -> Fill(refTime[0] - refTime[1]);
      h_refTimeTDC[1] -> Fill(refTime[0] - refTime[2]);
      h_refTimeTDC[2] -> Fill(refTime[0] - refTime[3]);

      h_refTimeTDC[3] -> Fill(refTime[3] - refTime[4]);
      h_refTimeTDC[4] -> Fill(refTime[3] - refTime[5]);
      h_refTimeTDC[5] -> Fill(refTime[3] - refTime[6]);

      //scint ref ch.no = 1176, sint = 1177

      //cout<<refTime[0]<<"\t"<<refTime[5]<<"\t"<<refTime[9]<<"\t"<<leadTimes[0][0]<<"\t"<<leadTimes[1][0]<<"\t"<<leadTimes[2][0]<<"\t"<<leadTimes[3][0]<<endl;    
      //   printf("Scinti : %lf, %lf, %lf\n",refTime[0],leadTimes[0][0],leadTimes[1][0]);

      //printf(" 0:%lf  1:%lf  2: %lf  3: %lf  4: %lf 5: %lf  6: %lf  7: %lf\n", refTime[0], refTime[1], refTime[2], refTime[3], refTime[4],refTime[5],refTime[6],refTime[7] ); 
      //printf(" 0 - 1 :%lf  0 - 2 :%lf  0 - 3: %lf  4 - 5: %lf\n", refTime[0] - refTime[1], refTime[0] - refTime[2], refTime[0] - refTime[3],refTime[4] - refTime[5] );        
      //cout<<"Processing begins"<<endl;

      //printf("ref scint: %lf,  scint time : %lf\n",leadTimes[1176][0],leadTimes[1177][0]);

      //printf("6440 ( 0 ) : %lf,  6440 ( 1 ) : %lf , 6440 ( 2 ) : %lf, 6440 ( 3 ) : %lf\n",leadTimes[0][0],leadTimes[1][0],leadTimes[2][0],leadTimes[3][0]);

      int tdc = 0;
      int module = 0;
      int plane = 0;
      int cell = 0;
      int tdc_ref_indx = 0;
      int tdc_id[6] = {
         0,
         4,
         8,
         12,
         16,
         20
      };
      int str_in_mod = 0;

      double rpc_ref[4];
      double rpc_hit[5];

      for (int rp = 0; rp < 4; rp++) {
         rpc_ref[rp] = 0;
      }
      for (int rp = 0; rp < 5; rp++) {
         rpc_hit[rp] = 0;
      }
      scint_ref = refTime[24];
      rpc_ref[0] = refTime[41];
      rpc_ref[1] = refTime[42];
      rpc_ref[2] = refTime[43];
      rpc_ref[3] = refTime[44];
      int rpc_idx = 0;
      for (int rk = 41 * 49; rk < 44 * 49; rk++) {

         if (rk % 49 != 0) {
            int rhits = (leadTimesNum[rk] < leadTimesNum[rk + 1]) ? leadTimesNum[rk] : leadTimesNum[rk + 1];
            if (leadTimesNum[rk] > 0 && leadTimesNum[rk] > 0) {
               for (int rh = 0; rh < rhits; rh++) {
                  //cout<<rk<<"\t"<<leadTimes[rk][rh]<<"\t"<<leadTimes[rk+1][rh]<<"\t"<<(leadTimes[rk][rh] + leadTimes[rk+1][rh]) /2<<"\t"<<leadTimes[rk][rh] - leadTimes[rk+1][rh]<<endl;
                  if (rpc_idx < 5) rpc_hit[rpc_idx++] = (leadTimes[rk][rh] + leadTimes[rk + 1][rh]) / 2;
               }
            }
            rk++;
         }
      }

      //for(int t=0; t<NUMBER_OF_TDCS; t++){tdc_id[t]=0;}

      vector < float > vec_lt;
	vector < float > vec_tot;
      vec_lt.clear();
      vec_tot.clear();
      float mean_lt=0;
      float sum_lt=0;
      int lt_count=0;

      for (int k = 0; k < CHANNELS; k++) {
         tdc = floor(k / 49);
         //    tdc_ref_indx = floor(tdc / 4);

         if (k > 2000) {
            for (int r = 0; r < leadTimesNum[k]; r++) {
               //cout<<"rpc  :"<<tdc<<"\t"<<refTime[tdc]<<endl;
               //cout<< k <<"\t"<< tdc <<"\t"<< leadTimes[k][r]<<endl;
               h_rpc_lt -> Fill(leadTimes[k][r]);

            }
         }
         //  ref_diff = (scint_ref - refTime[tdc]) - (refTime[tdc_id[tdc_ref_indx]] - refTime[0]);

         //cout<<ref_diff<<endl;

         if ((k % 49 != 0) and rpc_hit[0]!=0 ) {

            loc = det -> GetDetectorLocFromTDCChannel(k);
            //if(loc.layer==4)cout<<loc.layer<<"\t"<<k<<"\t"<<tdc<<endl;
            if (loc.layer > 0) {

               module = ((loc.layer - 1) * 8) + (1 + floor((loc.straw - 1) / 32));

               if (loc.layer == 1 || loc.layer == 3 || loc.layer == 5 || loc.layer == 7) {
                  if (loc.straw % 2 == 0) {
                     cell = 0;
                  } else {
                     cell = 1;
                  }
               } else if (loc.layer == 2 || loc.layer == 4 || loc.layer == 6 || loc.layer == 8) {
                  if (loc.straw % 2 == 0) {
                     cell = 1;
                  } else {
                     cell = 0;
                  }
               }

               plane = (loc.layer - 1) * 2 + cell;
               str_in_mod = loc.straw - (32 * floor(loc.straw / 32));

               for (int l = 0; l < leadTimesNum[k]; l++) {
                  Tot[k][TotNum[k]] = (trailTimes[k][l] - leadTimes[k][l]);
                  //cout<<k<<"\t"<<leadTimes[k][l] <<"\t"<<trailTimes[k][l] <<"\t"<<Tot[k][TotNum[k]]<<endl;

                  h_leadTime -> Fill(leadTimes[k][l]);
                  h_trailTime -> Fill(trailTimes[k][l]);
                  // h_leadTimeVsChannel->Fill(leadTimes[k][l],k);
                  //printf("%lf\n",leadTimes[k][l]);
                  h_Layer_leadTime[loc.layer - 1] -> Fill(leadTimes[k][l]);
                  h_Layer_trailTime[loc.layer - 1] -> Fill(trailTimes[k][l]);
                  //  h_Layer_leadTimeVsChannel[loc.layer-1]->Fill(leadTimes[k][l],k);

                  h_Module_leadTime[module - 1] -> Fill(leadTimes[k][l]);
                  h_Module_trailTime[module - 1] -> Fill(trailTimes[k][l]);
                  //  h_Module_leadTimeVsChannel[module-1]->Fill(leadTimes[k][l],k);
                  //cout<<"sts :"<<tdc<<"\t"<<refTime[tdc]+rpc_ref[0]<<"\t"<<refTime[tdc]+rpc_ref[1]<<"\t"<<refTime[tdc]+rpc_ref[2]<<"\t"<<refTime[tdc]+rpc_ref[3]<<endl;
                  //cout<<"Layer :"<<loc.layer<<"\t"<<trailTimesNum[k] <<"\t"<< leadTimesNum[k]<<"\t"<<leadTimes[k][l] <<"\t"<< trailTimes[k][l]<<"\t"<<Tot[k][TotNum[k]]<<endl;

                  //cout<<"Layer : "<<loc.layer<<"\t"<<loc.straw<<"\t"<<str_in_mod<<endl;

                  //if (trailTimesNum[k] == leadTimesNum[k] && leadTimes[k][l] < trailTimes[k][l])  {
                  if (trailTimesNum[k] == leadTimesNum[k]) {
                     //drifttime = ( scint_time - leadTimes[k][l] ) - ref_diff;
                     drifttime =  leadTimes[k][l] - rpc_hit[0];
                     //printf("%lf\n",drifttime);

                     if(loc.layer <5){
                     vec_lt.push_back(leadTimes[k][l]);
			vec_tot.push_back(Tot[k][TotNum[k]]);
                     sum_lt+=leadTimes[k][l];
                     lt_count++;
                     h_dummy_dt -> Fill(drifttime);
                     }

                 //    if ((drifttime >= -500 and drifttime <= 200) and(Tot[k][TotNum[k]] >= 0 and Tot[k][TotNum[k]] <= 600)) {
                        h_Tot -> Fill(Tot[k][TotNum[k]]); //General
                        //  h_driftTime->Fill(0);
                        
                        h_TotVsChannel -> Fill(Tot[k][TotNum[k]], k);
                        //  h_driftTimeVsChannel->Fill(drifttime,k);

                        h_Layer_Tot[loc.layer - 1] -> Fill(Tot[k][TotNum[k]]); //Layers
                        //  h_Layer_driftTime[loc.layer-1]->Fill(0);
                        //  h_Layer_driftTimeVsTOT[loc.layer-1]->Fill(drifttime,Tot[k][TotNum[k]]);
                        //   h_Layer_TotVsChannel[loc.layer-1]->Fill(Tot[k][TotNum[k]],k);
                        //h_Layer_TotVsStraw[loc.layer-1]->Fill(Tot[k][TotNum[k]],loc.straw);
                        //  h_Layer_driftTimeVsChannel[loc.layer-1]->Fill(drifttime,loc.straw);

                        h_Module_Tot[module - 1] -> Fill(Tot[k][TotNum[k]]); //Modules
                        //  h_Module_driftTime[module-1]->Fill(0);
                        //  h_Module_driftTimeVsTOT[module-1]->Fill(drifttime,Tot[k][TotNum[k]]);
                        //h_Module_TotVsChannel[module-1]->Fill(Tot[k][TotNum[k]],k);
                        //  h_Module_TotVsStraw[module-1]->Fill(Tot[k][TotNum[k]],str_in_mod);
                        //  h_Module_driftTimeVsChannel[module-1]->Fill(drifttime,str_in_mod);
                        //cout<<"2) :"<<loc.layer<<"\t"<<Tot[k][TotNum[k]]<<endl;
                        TotNum[k]++;
                        TotMod[module - 1]++;
                        TotLay[loc.layer - 1]++;
                        TotPlane[plane]++;
                        //TotStraw[ ((loc.layer -1) * 256) + loc.straw ]++;
                        TotStraw[loc.layer - 1][loc.straw]++;
                        (module % 8 == 0) ? h_strawMult -> Fill(((loc.layer - 1) * 256) + loc.straw - 128) : h_strawMult -> Fill(((loc.layer - 1) * 256) + loc.straw);

                        int pc_idx = 0;
                        double dt = 0;
                        double DT = 0;

                        /*
                        for(int pc=0; pc<5; pc++){
                        	//cout<<"DT :"<<pc<<"::"<<leadTimes[k][l] - rpc_hit[pc]<<"\t";
                        	dt = leadTimes[k][l] - rpc_hit[pc];

                        	if(pc==0){DT=rpc_hit[pc]-leadTimes[k][l];}
                        	else if(pc>0 && (rpc_hit[pc]-leadTimes[k][l])<DT ){
                        		DT = rpc_hit[pc]-leadTimes[k][l];
                        	}
                        	else DT = DT;
                        }
                        cout<<DT<<endl;*/
                        

                        ///////////////////////\ Geometry /\//////////////////

                        if (loc.straw % 2 == 0) {
                           h_FT_Geo -> Fill((loc.straw), loc.layer);
                        } else
                           h_FT_Geo -> Fill(loc.straw, loc.layer + 0.5);

                        if (loc.straw % 2 == 0) {
                           h_FT_Geo2 -> Fill((loc.straw), loc.layer);
                        } else
                           h_FT_Geo2 -> Fill(loc.straw, loc.layer + 0.5);

                        //h_geo->Fill(loc.straw, loc.layer);

                        ///////////////////////////////////////////////////////

                   //  } //window on DT and TOT
                  } //condition on LT & TT
               } //loop over hits  
               h_channelVsHits -> Fill(k, TotNum[k]);
            } //condition on loc.layer
         }
      } //loop over channels

      int hit_mult = 0;
      int mod_mult = 0;
      int plane_mult = 0;
      int layer_mult = 0;
      int stef1_mult[LAYERS];
      int stef2_mult[LAYERS];

	for(int st=0; st<LAYERS; st++){
		stef1_mult[st]=0;
		stef2_mult[st]=0;
	}

      int sts2_pmult = 0;
      int sts1_pmult = 0;
      int s2_pm = 0;
      int s1_pm = 0;
      bool s1_t = false;
      bool s2_t = false;

      for (int ly = 0; ly < LAYERS; ly++) {
         stef1_mult[ly] = 0;
         stef1_mult[ly] = 0;
      }

      for (int ht = 0; ht < CHANNELS; ht++) {
         if (TotNum[ht] > 0) {
            hit_mult++;
         }
      }
      for (int md = 0; md < MODULES; md++) {
         if (TotMod[md] > 0) {
            mod_mult++;
         }
      }
      for (int pl = 0; pl < LAYERS * 2; pl++) {
         if (TotPlane[pl] > 0) {
            plane_mult++;
         }

         if (pl < 8 && TotPlane[0] > 0 && TotPlane[7] > 0) {
            s2_t = true;
            if (TotPlane[pl] > 0) sts2_pmult++;

         }
         if (pl > 7 && TotPlane[8] > 0 && TotPlane[15] > 0) {
            s1_t = true;
            if (TotPlane[pl] > 0) sts1_pmult++;

         }

      }
      for (int ly = 0; ly < LAYERS; ly++) {
         if (TotLay[ly] > 0) {
            layer_mult++;
         }
         for (int j = 0; j < STRAWS; j++) {
            if (TotStraw[ly][j] > 0 and ly<4) stef2_mult[ly]++;
         }
      }

      if (s2_t == true) {
         for (int ly = 0; ly < 4; ly++) {
            h_sts2_eff_Lstraws -> Fill(ly + 1, stef2_mult[ly]);
         }
      }

      if (s1_t == true) {
         for (int ly = 4; ly < LAYERS; ly++) {
           // h_sts1_eff_Lstraws -> Fill(ly - 3, stef1_mult[ly]);
         }
      }
      h_layerMultiplicity -> Fill(layer_mult);
      h_planeMultiplicity -> Fill(plane_mult);
      h_hitMultiplicity -> Fill(hit_mult);
      //h_strawMult->Fill(straw_mult);
      h_sts1_peff -> Fill(sts1_pmult - 2);
      h_sts2_peff -> Fill(sts2_pmult - 2);
      if (s2_t == true) {
         h_sts_entries -> Fill(2);
         h_rpc_dt -> Add(h_dummy_dt);
      }
      if (s1_t == true) h_sts_entries -> Fill(1);
      h_dummy_dt -> Reset("ICESM");

      mean_lt= sum_lt/lt_count;
      if (vec_lt.size() > 1) {
         for (int v = 1; v < vec_lt.size(); v++) {

           // h_lt_diff -> Fill(mean_lt - vec_lt[v]);
		 h_lt_diff -> Fill(vec_lt[0] - vec_lt[v]);
		
         }
      }
	if(vec_lt.size() > 4 and s2_t == true){
		for (int v = 0; v < vec_lt.size(); v++) {

        		h_sts2_dt -> Fill( vec_lt[v] - mean_lt );
			h_driftTimeVsTOT->Fill( vec_lt[v] - mean_lt,vec_tot[v]);
        	}
	}

      //cout << "\t" << vec_lt.size() << endl;
      //h_sts1_eff_straws->Fill(stef1_mult);
      //h_sts2_eff_straws->Fill(stef2_mult);

      // } //process

      return true;
   }

   virtual void UserPostLoop() {

   }
};

void second() {
   gSystem -> Load("libGeom");
   LPetProcessor * proc = new LPetProcessor();
}

detLoc Detector::GetDetectorLocFromTDCChannel(int channel) {
   return detMap[channel];
}

Detector::Detector() {}
