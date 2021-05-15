function KeyPressCallback(~, event)

global speed;
global steer;

mode = event.Character;

switch mode
    
    % ESC
    case char(27)
       speed = 0;
       
    % 전진 방향키   
    case char(30)
        speed = speed + 0.1;
        
    % 후진  
    case char(31)
        speed = speed - 0.1;
    
    % 좌측
    case(28)
        steer = steer - 0.1;
        
    % 우측    
    case(29)
        steer = steer + 0.1;
    
    % 스페이스바
    case(32)
        speed = 0;
        steer = 0;
        
end
