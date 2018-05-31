pkg load instrument-control;

arduinoSerial = serial('COM4');
set(arduinoSerial, 'baudrate', 115200); % See List Below
set(arduinoSerial, 'bytesize', 8); % 5, 6, 7 or 8
set(arduinoSerial, 'parity', 'n'); % 'n' or 'y'
set(arduinoSerial, 'stopbits', 1); % 1 or 2

fopen(arduinoSerial);
upLimit = 50;
radiusMatrix = zeros(200,upLimit);
srl_flush(arduinoSerial);
srl_write(arduinoSerial,'b');
## including polynomial estimation function
##y = -1E-06x3 + 0.0016x2 - 0.7659x + 150.06 

 for zCount = 1:upLimit
  srl_write(arduinoSerial,'l');
  tmpArray = srl_read(arduinoSerial,200);
  tmpArray = double(tmpArray);
  radiusMatrix(:,zCount) = 46180*tmpArray.^(-1.282);
  end
 
 fclose(arduinoSerial);
 
 ##polar to cartesian coordinates
 
 for( i = 1:200 )
  theta = i * 1.8 * 0.0174532;
  X(i,:) = (radiusMatrix(i,:) - 160).*cos(theta);
  Y(i,:) = (radiusMatrix(i,:) - 160).*sin(theta);
 end
   Z = 1:49800;
 mesh(X(:,2:end),Y(:,2:end),Z)
 
 