function disp_listener(s,e)
set_param(get_param(s.BlockHandle, 'Parent'),  'MaskDisplay', sprintf('disp("Value=%f");',s.InputPort(1).Data));
end

