function ParameterExampleTestMain()
ModelName = 'ParameterExampleTest';
RampModelName = [ModelName '/Ramp'];
SFunModelName = [ModelName '/S-Function'];
open_system(ModelName);

RampSlope = str2double(get_param(RampModelName, 'slope'));
StartTime = str2double(get_param(RampModelName, 'start'));
InitialOutput = str2double(get_param(RampModelName, 'InitialOutput'));


% Test case 1: Constant parameter
Gain = 1;
set_param(SFunModelName, 'Parameters', [num2str(Gain) ', ''Test''']);
out = sim(ModelName);
data = out.result{1}.Values.Data;
time = out.result{1}.Values.Time;
expected = (((time-StartTime) >= 0) .* (time-StartTime)  .* RampSlope +InitialOutput) * Gain;

close_system(ModelName, 0);

assert(all(expected == data), 'Test Failed');

end