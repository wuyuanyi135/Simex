TestDirs = dir('test');

for i = 1 : numel(TestDirs)
   TestDir = TestDirs(i);
   
   if regexp(TestDir.name, '[a-z A-Z 1-9]') & TestDir.isdir 
      TestName = [TestDir.name, 'TestMain'];
      try
          run(TestName);
          fprintf('[Test %s passed]\n', TestName);
      catch e
          fprintf('[Test %s failed: %s]\n', TestName, e.message);
      end
   end
       
end