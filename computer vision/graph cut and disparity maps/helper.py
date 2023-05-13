import cv2
import numpy as np


def convert_disparity_image_to_map(image_path, step=18):
    """
    Converts disparity image into disparity map.
    Args:
    image_path: Path to the ground truth disparity map
    step: Number of rows and columns to be cropped from top, bottom, left and right.

    Note that you are given ground truth disparity map for the left image.
    So the final disparity map will have negative values.
    Hence there is a negative sign at the end.
    """
    disp_image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE).astype(np.int32)
    cropped_disp_map = -(disp_image[step:-step, step:-step] * 16 // 255)
    return cropped_disp_map
