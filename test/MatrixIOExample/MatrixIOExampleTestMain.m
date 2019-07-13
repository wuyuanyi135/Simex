function MatrixIOExampleTestMain()
ModelName = 'MatrixIOExampleTest';
open_system(ModelName);

out = sim(ModelName);
data = out.result.Data(:,:,:,2);
expected = imread('peppers.png');
expected = expected(1:300, 1:500, 1:3);


close_system(ModelName, 0);

assert(all(expected == data, 'all'), 'Test Failed');

end