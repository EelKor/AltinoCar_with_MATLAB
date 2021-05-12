 for ii = 1:5
        raw_read = read(device,1,"uint8");
        
        if raw_read == 2
            recieve = read(device, 13, "uint8");
            
            if recieve(13) == 3
                % 센서값 추출 부분
                front_L = bitshift(recieve(1), 8) + recieve(2);
                front_M = bitshift(recieve(3), 8) + recieve(4);
                front_R = bitshift(recieve(5), 8) + recieve(6);
                right = bitshift(recieve(7), 8) + recieve(8);
                left = bitshift(recieve(9), 8) + recieve(10);
                rear = bitshift(recieve(11), 8) + recieve(12);
                
                result = [front_L, front_M, front_R, right, left, rear];
                disp(recieve);
                % 그래프 그리기
                %plot(front,'*')
                %drawnow;
                %axis square
                %axis([-500 500 -500 500]);
                
            else
                flush();
            end
            
        else
            flush();
        end
    end