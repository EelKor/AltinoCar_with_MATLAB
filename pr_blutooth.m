clear all; close all;
clc
% 블루투스 리스트 표시
devlist = bluetoothlist("Timeout",20)
device = bluetooth("0018E43524DC")
prompt = 'Type Hex Data to Send Car: ';
while 1
    command = input(prompt);
    disp(command)
    write(device,command,"int8");
    
    reply = readline(device)
    if reply ~= '0'
        for ii = 1:4
        reply = readline(device);
        disp(reply);
    
        ii = ii + 1;
        end
    end
 
    
end

