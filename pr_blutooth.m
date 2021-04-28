clear all; close all;
clc
% 블루투스 리스트 표시
devlist = bluetoothlist("Timeout",20)
device = bluetooth("0018E43524DC")
prompt = 'Type Hex Data to Send Car: ';
while 1
    command = input(prompt);
    disp(command)
    decision = input("Is it right data? y=1 n=0 : ");
    if decision == 1
        write(device,command,"int8");
    else
        disp("Re type");
    end
    
end

