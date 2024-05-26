# Video Google

```
open episode.mp4
```

```
ffmpeg -i episode.mp4 -vf select='gt(scene\,0.2)' -vsync vfr frames/%d.png
```

* The filter command `select=gt(scene,0.2)` selects the frames whose scene detection score is greater then 0.2
  * [fmpeg/filters#select](http://www.ffmpeg.org/ffmpeg-filters.html#select_002c-aselect)
* Video sync method - frames are passed through with their timestamp or dropped so as to prevent 2 frames from having the same timestamp.
