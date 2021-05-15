function KeyPressCallback(hobj, event)

global speed;
global steer;

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
        speed = speed + 10;
        
    % 후진  
    case char(31)
        if speed > -300
        speed = -300;
        end
        speed = speed - 10;
    
    % 좌측
    case(28)
        steer = steer - 15;
        if steer < -127
            steer = -127;
        end
        
    % 우측    
    case(29)
        steer = steer + 15;
        if steer > 127
            steer = 127;
        end
    
    % 스페이스바
    case(32)
        speed = 0;
        steer = 0;
        
end
