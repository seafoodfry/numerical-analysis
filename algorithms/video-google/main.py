import cv2
import os
import subprocess
from pytube import YouTube


sift = cv2.SIFT_create()


if __name__ == "__main__":
    friends = "https://www.youtube.com/watch?v=HZlx7yrDu3c"
    yt = YouTube(friends)
    yt.streams.filter(progressive=True, file_extension="mp4").order_by("resolution").desc().first().download()

    os.rename(yt.title+".mp4", "episode.mp4")
    os.makedirs("frames", exist_ok=True)

    subprocess.run(["ffmpeg", "-i", "episode.mp4", "-vf", "select='gt(scene\,0.2)'", "-vsync", "vfr", "frames/%d.png"], capture_output=True, check=True)
