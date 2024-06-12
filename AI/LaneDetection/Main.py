
import cv2
import numpy as np
import requests
import time
from urllib.request import urlopen
from LaneLines import LaneLines
from PerspectiveTransformation import PerspectiveTransformation
from Thresholding import Thresholding
from CameraCalibration import CameraCalibration
from queue import Queue
import threading


class FindLaneLines:
    def __init__(self):
        """ Init Application"""
        # Initialize camera calibration object
        self.calibration = CameraCalibration('camera_cal', 9, 6)  
        # Initialize thresholding object
        self.thresholding = Thresholding()  
        # Initialize perspective transformation object
        self.transform = PerspectiveTransformation()
        # Initialize lane lines detection object
        self.lanelines = LaneLines()  
    def forward(self, img):
        out_img = np.copy(img)  # Create a copy of the input image

        # Undistort the image using camera calibration
        img = self.calibration.undistort(img)
        # Apply perspective transformation to convert image to bird-eye view
        img = self.transform.forward(img)
        # Apply thresholding to extract the lane line
        img = self.thresholding.forward(img)
        # Detect lane lines and motion control
        img = self.lanelines.forward(img)
        # Reverse perspective transformation to normal image
        img = self.transform.backward(img)

        # Combine the original and processed images
        out_img = cv2.addWeighted(out_img, 1, img, 0.6, 0)
        out_img = self.lanelines.plot(out_img)  # Visulize detected lane lines
        return out_img

     # Process the image in real-time
    def process_image_rt(self, img):
        out_img = self.forward(img)
        return out_img


find_lane_lines = FindLaneLines()
# URL to fetch images from the camera
url_image = r'http://192.168.109.105/capture'
# URL for sending commands
url_cmd = "http://192.168.109.105:81/message"
speed_cmd = 35  # Speed for vehicle
msg_to_dir_cmd = {
    "Straight": 1,
    "Left": 7,
    "Right": 5,
    "Stop": 9,
}  # Mapping of lane direction messages to command codes
queue_image = Queue()  # Create a thread-safe queue for storing images


# Get image from server of the carkit
def get_image():
    while True:
        try:
            img = urlopen(url_image, timeout=5)  # Open the URL to fetch image
            # Convert image data to NumPy array
            imgnp = np.asarray(bytearray(img.read()), dtype="uint8")
            img = cv2.imdecode(imgnp, -1)  # Decode the image
            img = cv2.resize(img, (1280, 720))  # Resize the image
            queue_image.put(img)  # Put the image into the queue
            # Check if the queue size exceeds the maximum limit
            if queue_image.qsize() >= 3:
                _ = queue_image.get()  # Pop the oldest image
        except Exception as e:
            print("Error fetching image:", e)
            time.sleep(1)  # Wait for 1 second before retrying

# Test the getting image process of carkit


def display_image():
    prev_time = time.time()  # Record the previous time
    while True:
        try:
            img = queue_image.get()  # Get an image from the queue
            img = find_lane_lines.process_image_rt(
                img)  # Process the image in real-time

            current_time = time.time()  # Record the current time
            fps = 1 / (current_time - prev_time)  # Calculate frames per second
            prev_time = current_time  # Update the previous time

            cv2.putText(img, f"FPS: {int(fps)}", (10, 30),
                        # Add FPS text to the image
                        cv2.FONT_HERSHEY_PLAIN, 2, (0, 0, 255), 2)
            cv2.imshow("Detected Image", img)  # Display the image

            if cv2.waitKey(1) & 0xFF == ord('q'):  # Check for 'q' key press to exit
                break
        except Exception as e:
            print("Error:", e)  # Print any errors

# Process image to extract line and motion control


def process_image():
    prev_time = time.time()  # Record the previous time
    while True:
        try:
            img = queue_image.get()  # Get an image from the queue
            current_time = time.time()  # Record the current time
            fps = 1 / (current_time - prev_time)  # Calculate frames per second
            prev_time = current_time  # Update the previous time
            # Process the image in real-time
            img = find_lane_lines.process_image_rt(img)
            msg = find_lane_lines.lanelines.msg  # Get the lane direction message
        except Exception as e:
            print("Error processing image:", e)  # Print errors
            cv2.putText(img, f"Not found", (150, 30),
                        # Add "Not found" text to the image
                        cv2.FONT_HERSHEY_PLAIN, 2, (0, 0, 255), 2)
            msg = "Stop"  # Set message to "Stop"

        # Get the command code corresponding to the message
        dir_cmd = msg_to_dir_cmd.get(msg, 9)
        data = {"Cmd": f"{dir_cmd} {speed_cmd}"}  # Construct the command data
        print(data)  # Print the command data
        # Send command to the vehicle
        response = requests.post(url_cmd, data=data)
        print(response.text)  # Print the response

        cv2.putText(img, f"FPS: {int(fps)}", (10, 30),
                    # Add FPS text to the image
                    cv2.FONT_HERSHEY_PLAIN, 2, (0, 0, 255), 2)
        cv2.imshow("Detected Image", img)  # Display the image

        if cv2.waitKey(1) & 0xFF == ord('q'):  # Check for 'q' key press to exit
            msg = "Stop"  # Set default message to "Stop"
            # Get the command code corresponding to the message
            dir_cmd = msg_to_dir_cmd.get(msg, 9)
            # Construct the command data
            data = {"Cmd": f"{dir_cmd} {speed_cmd}"}
            # Send command to the vehicle
            response = requests.post(url_cmd, data=data)
            break  # Exit the loop


def main():

    # Create two threads
    thread1 = threading.Thread(target=get_image, name='Thread 1')
    thread2 = threading.Thread(target=process_image, name='Thread 2')

    # Start the threads
    thread1.start()
    thread2.start()

    # Wait for the threads to finish
    thread1.join()
    thread2.join()

    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
