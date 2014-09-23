rm $1/driver.out-00000000-0.000000.osg;
find $1 -maxdepth 1 -name "img*.png" -type f -print | xargs rm

# this code parses Links data to produce a target location
grep "CoM_x = " $1/out.log > $1/com.mat
rpl -q "]';" "" $1/com.mat
rpl -q "CoM_x = [" "" $1/com.mat

a=1
for i in $1/driver.out-*.osg; do
  new=$(printf "img%04d.png" ${a});
  let a=a+1;
  /Users/samzapo/Projects/Locomotion/Examples/Quadruped/render \
    $i -p 0.5 1.5 0.5 -t $(awk 'NR == n' n=${a} com.mat) $1/${new};
done

# number all images img0001.png, etc...
ffmpeg -r 25 -i $1/img%04d.png -f mp4 -q:v 0 -vcodec mpeg4 $1/movie.mp4
