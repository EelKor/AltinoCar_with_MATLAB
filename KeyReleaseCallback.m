function KeyReleaseCallback(~, event)

global speed;
global steer;

mode = event.Character;

switch mode
    case char(27)
       speed = 0;
        
    case(28)
        steer = 0;
        
    case(29)
        steer = 0;
        
end