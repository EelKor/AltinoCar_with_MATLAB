% Altino_Controller.m
% 작 성 자: 기계융합공학과 이승신(2018013246)
% 작 성 일: 2021-04-28 ~ 2021-05-17
% 설    명
% 사용자의 키보드 조작으로 알티노 조종 및 알티노의 IR센서값을 매트랩에
% 실시간 플로팅
% 
% 데이터 패킷 구조
% MATLAB -> Altino
% 9바이트 명령 패킷
% STX Speed_H Speed_L Steer_H Steer_L None None Clocksion ETX
%
% STX = 시작 바이트
%
% Speed_H = 입력된 10진수 스피드 값을 이진법으로 변환후 비트 쉬프트 연산실시
% Speed_L = 위와 같은 원리
% 더 자세한 내용은 AltinoCar의 소스코드 main.c 주석 참조
%
% Steer_H = Speed_H와 동일
% Steer_L = Speed_H와 동일
%
% None = 빈 공간 ( 향후 할당 가능 )1
%
% Clocksion = 액세서리 제어 (빵빵)
%
% ETX = 종료 바이트

clear all; close all;
clc
% 블루투스 리스트 표시
fprintf("******  ALTINO Control Program *****\n");
fprintf("Connecting...\n");
devlist = bluetoothlist("Timeout",20)
device = bluetooth("201603107014",1,"ByteOrder","little-endian","Timeout",1)


% 그래프 상수
plot_front = [0, 0, 0];
front_X = [-600 0 600];

plot_side = [-500, 0, 500];
right_X = [0, 0, 0];

% 전역변수 선언
global speed;
global steer;
global clocksion;
global L_keypress;
global R_keypress;
speed = 0;
steer = 0;
clocksion = uint8([0 0 0 0 0 0 0 0]);
L_keypress = 0;
R_keypress = 0;

speed_run = 0;
steer_run = 0;

% 조종 감도 조정 변수
global SteeringSensitivity;
global AccSensitivity;
AccSensitivity = 10;
SteeringSensitivity = 30;



% 그래프 생성
HndF = figure(1);
axes;
x = 0;

while 1
    % 왼쪽 방향키가 눌려 있지 않으면
    if L_keypress == 0 || R_keypress == 0
        % 조향값이 양수일 경우
        if steer > 0
            steer = steer - SteeringSensitivity;
            if steer < 0
                steer = 0;
            end
        
        % 조향값이 음수, 0 일 경우
        else 
            steer = steer + SteeringSensitivity;
            if steer > 0
                steer = 0;
            end
        end

    
    end
    
    % 알티노 속도 범위 -1000 ~ 1000
    % 시리얼 통신으로 음수는 전송하기 힘들어서
    % 0 ~ 2000으로 변환하여 전송
    speed_run = speed + 1000;
    speed_run = round(speed_run);
    
    % 1000을 하나의 바이트에 담기 불가능
    % High byte, Low byte로 데이터를 쪼개서 전송
    Speed_H = bitshift(speed_run, -7);
    Speed_L = speed_run - bitshift(Speed_H, 7);
    
    % speed 와 동일
    steer_run = steer + 127;
    steer_run = round(steer_run);
    Steer_H = bitshift(steer_run, -7);
    Steer_L = steer_run - bitshift(Steer_H, 7);
    
    % 악세사리 제어
    Acc = 0;
    for i = 1:8
        Acc = Acc + clocksion(i)*2^(8-i);
    end
        
    % 입력받은 명령들을 패킷화
    command = [2 Speed_H Speed_L Steer_H Steer_L 65 65 Acc 3];
    
    status = 0;
    for COMcount = 1:10 
        
        % 블루투스 시리얼로 int8형식으로 전송
        write(device,command,"uint8");
        raw_read = read(device,1,"uint8");
        
        % 시작 바이트 인식
        if raw_read == 2
            recieve = read(device, 14, "uint8");
            if length(recieve) == 14
                % 종료 바이트 인식 -> 데이터 패킷 이상 유무 확인
                if recieve(14) == 3
                    % 센서값 추출 부분
                    status = recieve(1);
                    front_L = bitshift(recieve(2), 8) + recieve(3);
                    front_M = bitshift(recieve(4), 8) + recieve(5);
                    front_R = bitshift(recieve(6), 8) + recieve(7);
                     right = bitshift(recieve(8), 8) + recieve(9);
                    left = bitshift(recieve(10), 8) + recieve(11);
                    rear = bitshift(recieve(12), 8) + recieve(13);
                
                    % 센서값 표시
                    result = [status, front_L, front_M, front_R, right, left, rear];
                    %disp(result)
         
                  
                    % 그래프 생성
                    plot_front = [1000-front_L, 1000-front_M, 1000-front_R];
                    right_X = [1000-right, 1000-right, 1000-right];
                    left_X = [-1000+left, -1000+left, -1000+left];
                    plot_rear = [-1000+rear, -1000+rear, -1000+rear,];


                    plot(front_X,plot_front,'*-', right_X,plot_side,'*-', left_X,plot_side,'*-', front_X, plot_rear,'*-' );
                    axis manual;
                    axis([-1100 1100 -1100 1100]);
                    grid on;
                    drawnow;
                    fprintf("%d %d\n", steer, speed);
                    set(HndF,'KeyPressFcn', @KeyPressCallback, 'KeyReleaseFcn', @KeyReleaseCallback)
                
                    if status == 4
                        break;
                    end
                    
                % if recieve(14 == 3
                else
                    flush(device);
                end
            % if length(14) == 3    
            else
                flush(device);
            end
            
        % if raw_read    
        else
            flush(device);
        end
    end
 
end

