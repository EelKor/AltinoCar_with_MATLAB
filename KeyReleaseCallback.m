function KeyReleaseCallback(~, event)

global speed;
global clocksion;
global L_keypress;
global R_keypress;

mode = event.Character;

switch mode
    % ESC 버튼
    case char(27)
       speed = 0;
        
    
    % Q 버튼
    case(113)
        clocksion = bitand(clocksion, uint8([0 0 0 0 0 0 1 0]));
        
        
    % 좌측
    case(28)
        % 키 눌림 확인
        L_keypress = 0;
        
        
    % 우측    
    case(29)
        % 키 눌림 확인
        R_keypress = 0;
end

        
end