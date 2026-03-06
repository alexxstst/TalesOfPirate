C_NORMAL    = 0  -- Normal mode   
C_NEAR      = 1  -- Close mode distance is closer than normal mode
C_HIGHSPEED = 2  -- High speed mode fixed lens, non-rotating
C_SHIP      = 3  -- Sea mode on a boat, far away

local iMaxDrawRange = 80

CameraRangeXY( C_NORMAL, 50, 35)
CameraRangeZ( C_NORMAL, 10, 50)
CameraRangeFOV( C_NORMAL, 12, 32)
CameraEnableRotate( C_NORMAL, 1)
CameraShowSize( C_NORMAL, iMaxDrawRange, iMaxDrawRange)

CameraRangeXY( C_NEAR, 50, 35)
CameraRangeZ( C_NEAR, 10, 50)
CameraRangeFOV( C_NEAR, 12, 32)
CameraEnableRotate( C_NEAR, 1)
CameraShowSize( C_NEAR, iMaxDrawRange, iMaxDrawRange)

CameraRangeXY( C_HIGHSPEED, 50, 35)
CameraRangeZ( C_HIGHSPEED, 10, 50)
CameraRangeFOV( C_HIGHSPEED, 12, 32)
CameraEnableRotate( C_HIGHSPEED, 1)
CameraShowSize( C_HIGHSPEED, iMaxDrawRange, iMaxDrawRange) 

CameraRangeXY( C_SHIP, 50, 35)
CameraRangeZ( C_SHIP, 10, 50)
CameraRangeFOV( C_SHIP, 12, 32)
CameraEnableRotate( C_SHIP, 1)
CameraShowSize( C_SHIP, iMaxDrawRange, iMaxDrawRange)

--
function LoadCameraConfig()
  
end
