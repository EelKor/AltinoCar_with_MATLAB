clear
close all;

figure(1);
axes;

% 제어 변수 부분
speed = 0;
steer = 0;

% 블루투스 연결 부분
fprintf(" 블루투스 연결 중 .....");
devlist = bluetoothlist("Timeout",20)
device = bluetooth("0018E43524DC")

% 원격 제어
while 1
key_press = waitforbuttonpress

       val_0 = double(get(gcf,'CurrentCharacter'));
       switch val_0
           % 전진 방향키
           case 30
               speed = speed + 10;
           
           % 후진 방향키
           case 31
               speed = speed - 10;
           
           % 스페이스바 정지
           case 32
               speed = 0;
               
       end

               
             
  
        
    
end
