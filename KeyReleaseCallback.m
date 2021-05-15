function KeyReleaseCallback(~, event)

global speed;
global steer;
global clocksion;

mode = event.Character;

switch mode
    case char(27)
       speed = 0;
        
    case(28)
        steer = steer + 10;
        if steer > 0
            steer = 0;
        end
        
        
    case(29)
        steer = steer - 10;
        if steer < 0
            steer = 0;
        end
    
    % Q 버튼
    case(113)
        clocksion = 0;
        
end