clear all; close all;
global myVariable;
global myVariableX;
myVariable = 0;
myVariableX = 0;

HndF = figure(1);
axes;
x = 0;

while 1
    fprintf("%d %d\n", myVariableX, myVariable);
    plot(myVariableX,myVariable,'b*')
    axis manual;
    axis([-10 10 -10 10]);
    grid on;
    drawnow;
    
    set(HndF,'KeyPressFcn', @KeyPressCallback, 'KeyReleaseFcn', @KeyReleaseCallback)
end

