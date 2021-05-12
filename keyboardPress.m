clear
close all;

figure(1);
axes;

% 제어 변수 부분
speed = 1000;
steer = 127;

% 블루투스 연결 부분
fprintf(" 블루투스 연결 중 .....");
devlist = bluetoothlist("Timeout",20)
device = bluetooth("201603107014")

% 원격 제어
while 1
key_press = waitforbuttonpress;

       val_0 = double(get(gcf,'CurrentCharacter'));
       switch val_0
           % 전진 방향키
           case 30
             speed = speed + 50;
             if speed > 1500
                 speed = 1500;
             end
    
             % 1000을 하나의 바이트에 담기 불가능
             % High byte, Low byte로 데이터를 쪼개서 전송
             Speed_H = bitshift(speed, -7);
             Speed_L = speed - bitshift(Speed_H, 7);
             
             steer = 127;
             Steer_H = bitshift(steer, -7);
             Steer_L = steer - bitshift(Steer_H, 7);
    
             % 입력받은 명령들을 패킷화
             command = [2 Speed_H Speed_L Steer_H Steer_L 65 65 65 3];
    
             % 블루투스 시리얼로 int8형식으로 전송
              write(device,command,"int8");
              
              for ii = 1:5
        raw_read = read(device,1,"uint8");
        
        % 시작 바이트 인식
        if raw_read == 2
            recieve = read(device, 13, "uint8");
            
            % 종료 바이트 인식 -> 데이터 패킷 이상 유무 확인
            if recieve(13) == 3
                % 센서값 추출 부분
                front_L = bitshift(recieve(1), 8) + recieve(2);
                front_M = bitshift(recieve(3), 8) + recieve(4);
                front_R = bitshift(recieve(5), 8) + recieve(6);
                right = bitshift(recieve(7), 8) + recieve(8);
                left = bitshift(recieve(9), 8) + recieve(10);
                rear = bitshift(recieve(11), 8) + recieve(12);
                
                % 센서값 표시
                result = [front_L, front_M, front_R, right, left, rear];
                disp(result);
                
                % 그래프 그리기
                %plot(front,'*')
                %drawnow;
                %axis square
                %axis([-500 500 -500 500]);
                
                % 쌓여있는 오래된 버퍼 데이터 삭제
                flush(device);
                
            else
                flush(device);
            end
            
        else
            flush(device);
        end
    end
           
           % 후진 방향키
           case 31
               speed = speed - 50;
               if speed < 500
                   speed = 500;
               end
               % 1000을 하나의 바이트에 담기 불가능
             % High byte, Low byte로 데이터를 쪼개서 전송
             Speed_H = bitshift(speed, -7);
             Speed_L = speed - bitshift(Speed_H, 7);
             
             steer = 127;
             Steer_H = bitshift(steer, -7);
             Steer_L = steer - bitshift(Steer_H, 7);
    
             % 입력받은 명령들을 패킷화
             command = [2 Speed_H Speed_L Steer_H Steer_L 65 65 65 3];
    
             % 블루투스 시리얼로 int8형식으로 전송
              write(device,command,"int8");
              
              for ii = 1:5
        raw_read = read(device,1,"uint8");
        
        % 시작 바이트 인식
        if raw_read == 2
            recieve = read(device, 13, "uint8");
            
            % 종료 바이트 인식 -> 데이터 패킷 이상 유무 확인
            if recieve(13) == 3
                % 센서값 추출 부분
                front_L = bitshift(recieve(1), 8) + recieve(2);
                front_M = bitshift(recieve(3), 8) + recieve(4);
                front_R = bitshift(recieve(5), 8) + recieve(6);
                right = bitshift(recieve(7), 8) + recieve(8);
                left = bitshift(recieve(9), 8) + recieve(10);
                rear = bitshift(recieve(11), 8) + recieve(12);
                
                % 센서값 표시
                result = [front_L, front_M, front_R, right, left, rear];
                disp(result);
                
                % 그래프 그리기
                %plot(front,'*')
                %drawnow;
                %axis square
                %axis([-500 500 -500 500]);
                
                % 쌓여있는 오래된 버퍼 데이터 삭제
                flush(device);
                
            else
                flush(device);
            end
            
        else
            flush(device);
        end
    end
           
           % 스페이스바 정지
           case 32
               speed = 0;
               
       end

               
             
  
        
    
end
