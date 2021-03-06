#include "../backend/main.hpp"

int main() {
  h = new HomeIO();
  signal(SIGINT, handleSignal);

  // IoProxy connects to IoServer.
  // IoServer acts as bridge between uC via UART/RS/USB/hardware using TCP protocol.
  // It gets command, send to hardware, get hardware response and trasmit it via TCP.
  // If you run everything on same server use address 'localhost'. It's default way.
  // If you want to connect to another server where is hardware connected and IoServer is run
  // type IP of this server
  // example:
  // h->ioProxy->address = "192.168.0.20";
  // default:
  // h->ioProxy->address = "localhost";
  h->ioProxy->address = "localhost";
  // please use default port
  h->ioProxy->port = 2002;

  // Now please add type of measurements which you want backend to fetch and
  // store in RAM buffer.

  MeasType *m;

  unsigned int i;

  for (i = 0; i < 60; i++) {
    m = new MeasType();
    m->name = "test_batt_" + std::to_string(i);
    m->unit = "V";
    m->command = '3';
    m->responseSize = 2;
    m->coefficientLinear = 0.0777126099706744868;
    m->coefficientOffset = 0;
    m->minTimeDiffToStore = 1000;
    m->maxTimeDiffToStore = 3600000;
    m->valueDiffToStore = 2.0;
    m->priority = 1;
    h->measTypeArray->add(m);
  }

  h->ioServer->port = "/dev/ttyUSB0";

  // meas fetcher intervals
  h->measFetcher->betweenMeasInterval = 10000;
  h->measFetcher->cycleInterval = 50000;

  h->fileStorage->cycleInterval = 24*3600*1000;

  // actions
  ActionType *a;
  a = new ActionType();
  // this must be unique and will be used by frontend and overseers
  a->name = "start_total_brake";
  // ascii command to get measurement
  a->command = 'w';
  // number of bytes of response
  a->responseSize = 1;
  // "okay" response value
  a->responseOkay = 21;
  // add this measuremt type to measurement which will be fetched by this server
  h->actionTypeArray->add(a);

  a = new ActionType();
  // this must be unique and will be used by frontend and overseers
  a->name = "turn_off_brake";
  // ascii command to get measurement
  a->command = 'W';
  // number of bytes of response
  a->responseSize = 1;
  // "okay" response value
  a->responseOkay = 20;
  // add this measuremt type to measurement which will be fetched by this server
  h->actionTypeArray->add(a);


  a = new ActionType();
  a->name = "inverter_A_off"; // "output_2_off";
  a->command = 'b';
  a->responseSize = 1;
  a->responseOkay = 1;
  h->actionTypeArray->add(a);

  a = new ActionType();
  a->name = "inverter_B_off"; // "output_3_off";
  a->command = 'c';
  a->responseSize = 1;
  a->responseOkay = 2;
  h->actionTypeArray->add(a);

  a = new ActionType();
  a->name = "inverter_A_on"; // "output_2_on";
  a->command = 'B';
  a->responseSize = 1;
  a->responseOkay = 11;
  h->actionTypeArray->add(a);

  a = new ActionType();
  a->name = "inverter_B_on"; // "output_3_on";
  a->command = 'C';
  a->responseSize = 1;
  a->responseOkay = 12;
  h->actionTypeArray->add(a);



  // overseers intervals
  h->overseerArray->cycleInterval = 2000000;

  /*
  // overseers
  Overseer *o;
  o = new Overseer();
  // this must be unique and can be used to access overseer parameters in future
  o->name = "brake_on_high_batt_voltage";
  // string name of action
  o->actionName = "start_total_brake";
  // string name of measurement
  o->measName = "batt_u";
  // critical value
  o->thresholdValue = 46.5;
  // check if value is higher
  o->isMax = true;
  // use average value using X raw measurements
  o->windowSize = 300;
  // add this overseer type to list
  h->overseerArray->add(o);

  o = new Overseer();
  o->name = "no_brake_on_low_batt_voltage";
  o->actionName = "turn_off_brake";
  o->measName = "batt_u";
  o->thresholdValue = 29.0;
  o->isMax = false;
  o->windowSize = 100;
  h->overseerArray->add(o);

  o = new Overseer();
  o->name = "turn_on_inverter_A_on_high_coil";
  o->actionName = "inverter_A_on";
  o->measName = "coil_1_u";
  o->thresholdValue = 30.0;
  o->isMax = true;
  o->windowSize = 100;
  h->overseerArray->add(o);

  o = new Overseer();
  o->name = "turn_on_inverter_A_on_high_battery";
  o->actionName = "inverter_A_on";
  o->measName = "batt_u";
  o->thresholdValue = 36.0;
  o->isMax = true;
  o->windowSize = 200;
  h->overseerArray->add(o);

  o = new Overseer();
  o->name = "turn_off_inverter_A_on_low_coil";
  o->actionName = "inverter_A_off";
  o->measName = "coil_1_u";
  o->thresholdValue = 2.0;
  o->isMax = false;
  o->windowSize = 50;
  h->overseerArray->add(o);

  o = new Overseer();
  o->name = "turn_off_inverter_B_on_low_coil";
  o->actionName = "inverter_B_off";
  o->measName = "coil_1_u";
  o->thresholdValue = 2.0;
  o->isMax = false;
  o->windowSize = 50;
  h->overseerArray->add(o);

  o = new Overseer();
  o->name = "turn_on_inverter_B_on_high_battery";
  o->actionName = "inverter_B_on";
  o->measName = "batt_u";
  o->thresholdValue = 30.0;
  o->isMax = true;
  o->windowSize = 1000;
  h->overseerArray->add(o);

  o = new Overseer();
  o->name = "turn_off_inverter_B_on_low_battery";
  o->actionName = "inverter_B_off";
  o->measName = "batt_u";
  o->thresholdValue = 23.0;
  o->isMax = false;
  o->windowSize = 100;
  h->overseerArray->add(o);

  o = new Overseer();
  o->name = "turn_off_inverter_A_on_low_battery";
  o->actionName = "inverter_A_off";
  o->measName = "batt_u";
  o->thresholdValue = 23.0;
  o->isMax = false;
  o->windowSize = 100;
  h->overseerArray->add(o);
  */


  // settings which will only be usable in frontend
  h->frontendSettings->intervalCurrent = 1000;
  h->frontendSettings->intervalHistory = 3600*1000;
  h->frontendSettings->currentCoeff = 0.4;

  // WARNING
  // this announce measurements to global server used for statistics and uptime
  h->spy->enabled = false;
  h->spy->cycleInterval = 10*60*1000*1000; // 10 minutes
  h->spy->hiveHost = "http://hive.homeio.org.pl";
  h->spy->siteName = "lakie_dev";

  // buffer storage
  h->measBufferBackupStorage->cycleInterval = (long) 10*60*1000*1000; // 10 minutes
  h->measBufferBackupStorage->thresholdTimeRange = (long) 20*60*1000*1000; // 20 minutes
  h->measBufferBackupStorage->usDelay = 60*1000*1000; // 1 minute

  // ram buffer resize
  h->measFetcher->maxBufferSize = 2000000; // 4000000;

  // addons
  WindTurbineStatsAddon *wts = new WindTurbineStatsAddon;
  wts->measNameU = "batt_u";
  wts->measNameI = "i_gen_batt";
  wts->measCoil = "coil_1_u";
  wts->measResistor = "res_pwm_avg";
  h->addonsArray->addons.push_back(std::unique_ptr<AbstractAddon>(wts));

  // Now you can run everything of parts of backend.
  // TODO: way to disable IoServer
  h->start();

  return 0;
}
