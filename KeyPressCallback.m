function KeyPressCallback(hobj, event)

global speed;
global steer;
global clocksion;
global L_keypress;
global R_keypress;
global AccSensitivity;
global SteeringSensitivity;

mode = event.Character;

switch mode
    
    % ESC
    case char(27)
       close(hobj);
       
    % 전진 방향키   
    case char(30)
        if speed < 300
        speed = 300;
        end
        speed = speed + AccSensitivity;
        
    % 후진  
    case char(31)
        if speed > -300
        speed = -300;
        end
        speed = speed - AccSensitivity;
    
    % 좌측
    case(28)
        % 키 눌림 확인
        L_keypress = 1;
        steer = steer - SteeringSensitivity;
        if steer < -127
            steer = -127;
        end
        
    % 우측    
    case(29)
        % 키 눌림 확인
        R_keypress = 1;
        steer = steer + SteeringSensitivity;
        if steer > 127
            steer = 127;
        end
    
    % 스페이스바
    case(32)
        speed = 0;
        steer = 0;
    
    % Q 버튼
    case(113)
        clocksion  = bitor(clocksion, uint8([0 0 0 0 0 0 0 1]));
    
    % 숫자1 = 전조등 on off
    case(49)
        if clocksion(7) == 1
            clocksion(7) = 0;
            
        else
            clocksion(7) = 1;
        end
        
            
        
        
        
end
