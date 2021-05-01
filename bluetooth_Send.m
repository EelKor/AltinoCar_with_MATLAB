clear all; close all;
clc
% 블루투스 리스트 표시
devlist = bluetoothlist("Timeout",20)
device = bluetooth("0018E43524DC")
prompt = 'Type Data to send Car: ';
while 1
    % 명령 입력받음
    command = input(prompt);
    % 입력 받은 명령 출력
    disp(command)
    % 블루투스 시리얼로 int8형식으로 전송
    write(device,command,"int8");
    
    % 알티노로 부터 응답 읽기
    for ii = 1:10
        
        reply = read(device,1,"string");
        switch reply
            case '0' 
                fprintf("reply = %s\n", reply);
                break;
            
            case 'e'
                fprintf("reply = %s\n", reply);
                break;
            
            case 's'
                fprintf("reply = %s\n", reply);
                break;
            
        end
    end
    
    
    % 알티노는 1바이트 응답을 하므로 그 뒤는 모두 가비지 값
    % 삭제
    flush(device);
 
end
