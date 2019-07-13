function SimpleExampleTestMain()
ModelName = 'SimpleExampleTest';
RampModelName = [ModelName '/Ramp'];
ConstantModelName = [ModelName '/Constant'];
open_system(ModelName);

RampSlope = str2double(get_param(RampModelName, 'slope'));
StartTime = str2double(get_param(RampModelName, 'start'));
InitialOutput = str2double(get_param(RampModelName, 'InitialOutput'));

ConstantValue = str2double(get_param(ConstantModelName, 'Value'));
out = sim(ModelName);
data = out.result{1}.Values.Data;
time = out.result{1}.Values.Time;
expected = ((time-StartTime) >= 0) .* (time-StartTime)  .* RampSlope +InitialOutput + ConstantValue;

close_system(ModelName, 0);


assert(all(expected == data), 'Test Failed');
end