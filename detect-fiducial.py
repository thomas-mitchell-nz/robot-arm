# fiducial.py

# Importing Libraries
import time
import sys
import serial
import numpy as np
import cv2
import cv2.aruco as aruco
import pyrealsense2 as rs

# Configure video streams
x_res = 640
y_res = 480
fps = 30
pipeline = rs.pipeline()
config = rs.config()
config.enable_stream(rs.stream.depth, x_res, y_res, rs.format.z16, fps)
config.enable_stream(rs.stream.color, x_res, y_res, rs.format.bgr8, fps)

# Start streaming
profile = pipeline.start(config)

# Create an align object
align = rs.align(rs.stream.color)

# Retrieve one of the fiducial dictionaries
dictionary = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_6X6_250)

# Load the two fiducial marker PNG files
marker1 = cv2.imread('fiducial1.png', cv2.IMREAD_GRAYSCALE)
marker2 = cv2.imread('fiducial2.png', cv2.IMREAD_GRAYSCALE)

# Set marker flags
marker1_flag = False
marker2_flag = False

# Get dimensions of template image
marker_height, marker_width = marker1.shape

# Open serial connection with Arduino
arduino = serial.Serial(port='COM11', baudrate=115200, timeout=.1)

# Normally these would be loaded from an external source.
camera_matrix = np.array([[  1.10203699e+03,   0.00000000e+00,   2.97856040e+02],
                          [  0.00000000e+00,   1.10715227e+03,   2.19618658e+02],
                          [  0.00000000e+00,   0.00000000e+00,   1.00000000e+00]])
distortion_coeff = np.array([7.978574896538845329e-02, 3.400042995004967317e+00, -1.786514214937548820e-02,
                            -3.217060871280347668e-03, -2.063856972981825777e+01])

def generate_marker(marker_id=24, size=200):
    # By default, generate marker #24 (200 pixels in size) from the dictionary
    marker = aruco.drawMarker(dictionary, marker_id, size)
    cv2.imwrite('fiducial_{}.png'.format(marker_id), marker)

def rotation_vector_to_euler(rotation_vector):
    # Convert an OpenCV-style rotation vector into Euler angles in degrees
    rotation_matrix, _ = cv2.Rodrigues(rotation_vector)
    euler_angles, _, _, _, _, _ = cv2.RQDecomp3x3(rotation_matrix)
    return euler_angles

def resize_window(window, corner_vec, factor):
    # Compute the size of the rectangular region defined by corners
    corners = [(int(c[0]), int(c[1])) for c in corner_vec]
    width = max([c[0] for c in corners]) - min([c[0] for c in corners])
    height = max([c[1] for c in corners]) - min([c[1] for c in corners])

    # Compute the new corners according to the scaling factor
    new_width = int(factor * width)
    new_height = int(factor * height)
    center_x = (max([c[0] for c in corners]) + min([c[0] for c in corners])) // 2
    center_y = (max([c[1] for c in corners]) + min([c[1] for c in corners])) // 2
    new_min_x = center_x - new_width // 2
    new_max_x = center_x + new_width // 2
    new_min_y = center_y - new_height // 2
    new_max_y = center_y + new_height // 2

    # Extract rectangular region defined by new corners
    region = window[new_min_y:new_max_y, new_min_x:new_max_x]

    # Return new region
    return region

while cv2.waitKey(1) < 0:
    # Wait for a coherent pair of frames: depth and color
    frames = pipeline.wait_for_frames()
    aligned_frames = align.process(frames)

    # Get aligned frames
    depth_frame = aligned_frames.get_depth_frame()
    color_frame = aligned_frames.get_color_frame()

    # Convert images to numpy arrays
    depth_image = np.asanyarray(depth_frame.get_data())
    color_image = np.asanyarray(color_frame.get_data())

    # Apply colormap on depth image for visibilty (image must be converted to 8-bit per pixel first)
    depth_colormap = cv2.applyColorMap(cv2.convertScaleAbs(depth_image, alpha=0.03), cv2.COLORMAP_JET)
    
    # Convert image from BGR color space to grayscale
    gray_frame = cv2.cvtColor(color_image, cv2.COLOR_BGR2GRAY)

    # Attempt to detect markers in the image
    corner_array, ids, rejected = aruco.detectMarkers(gray_frame, dictionary) 

    if ids is not None:  # Markers have been detected
        # Estimate the pose of the detected markers (assumes marker is 7cm wide)
        poses = aruco.estimatePoseSingleMarkers(corner_array, 0.035, camera_matrix, distortion_coeff)

        # Unpack poses into a rotation vector rvec and a translation vector tvec
        rvec, tvec = poses[0:2]

        # Draw the detected markers onto the image
        color_image = aruco.drawDetectedMarkers(color_image, corner_array, ids) 

        for i in range(len(ids)): # For each marker that has been detected
            # Extract camera position and orientation from poses
            x, y, z = np.squeeze(tvec[i])
            rx, ry, rz = rotation_vector_to_euler(np.squeeze(rvec[i]))

            # Draw a 3D axis for each marker position
            color_image = cv2.drawFrameAxes(color_image, camera_matrix, distortion_coeff, rvec[i], tvec[i], 0.035)
            ''' print("ID: {}\n\tTranslation Vector: x: {:6.2f}m, y: {:6.2f}m, z: {:6.2f}m\n\t \
            \Rotation Vector: rx: {:6.2f}deg, ry: {:6.2f}deg, rz: {:6.2f}deg".format(ids[i], x, y, z, rx, ry, rz))'''

            # Compute the new rectangle with 30% smaller dimensions
            depth_region = resize_window(depth_image, corner_array[i][0], 0.7)

            # Compute average depth within region
            average_depth = int(np.mean(depth_region))
            '''print("Average depth: ", average_depth)'''

            # Extract the fiducial marker from the grayscale image with 20% larger dimensions
            rect_gray = resize_window(gray_frame, corner_array[i][0], 1.2)

            # Check dimensions of the source image
            rect_height, rect_width = rect_gray.shape

            if (rect_height > 0 and rect_height < y_res and rect_width > 0 and rect_width < x_res):
                # Resize source image to match template size
                rect_gray = cv2.resize(rect_gray, (marker_width, marker_height))
                
                # Apply binary thresholding to convert image to black and white
                '''_, rect_gray = cv2.threshold(rect_gray, 70, 255, cv2.THRESH_BINARY)'''

                # Display cropped image
                cv2.imshow("cropped", rect_gray)

                # Try to match the first and second fiducial markers
                res1 = cv2.matchTemplate(rect_gray, marker1, cv2.TM_CCOEFF_NORMED)
                res2 = cv2.matchTemplate(rect_gray, marker2, cv2.TM_CCOEFF_NORMED)

                if max(res1) > 0.5: # If the detected marker matches the first fiducial marker
                    marker = '1'
                elif max(res2) > 0.5: # If the detected marker matches the second fiducial marker
                    marker = '2'
                else:
                    marker = 'null'

            if abs(x-.05) < 1e-2: # Marker aligned with the horizontal middle of the frame
                # Send signal to arduino if target has not already been detected
                if (marker == '1' and not marker1_flag):
                    str = "{} {}".format(marker, average_depth)
                    print('str: ', str)
                    arduino.write(bytes(str, 'utf-8'))
                    marker1_flag = True
                elif (marker == '2' and not marker2_flag):
                    str = "{} {}".format(marker, average_depth)
                    print('str: ', str)
                    arduino.write(bytes(str, 'utf-8'))
                    marker2_flag = True

    # Stack images horizontally
    images = np.hstack((color_image, depth_colormap))

    # Show images
    cv2.namedWindow('RealSense', cv2.WINDOW_AUTOSIZE)
    cv2.imshow('RealSense', images)

    # Pause program for 100 ms
    time.sleep(0.1)

# Stop the camera and close the GUI windows
pipeline.stop()
cv2.destroyAllWindows()