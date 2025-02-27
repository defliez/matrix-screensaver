# Matrix Screensaver

This is a project built in C, featuring two variations of matrix-themed screensavers.

The programs leverage ncurses, a C library that allows for creating and rendering terminal windows, as well as the Ghostty terminal which has features to open a terminal window in fullscreen with a transparent background. This can also be done with the use of other terminals. To turn it into a screensaver, this project uses hyprland and hypridle, although it is possible to use other environments and idle daemons.

### Previews

##### Infinite

https://github.com/user-attachments/assets/7eb8f512-609f-4a82-be59-f78c47782056

##### "WAKE UP"

https://github.com/user-attachments/assets/588a515d-2754-493f-bfb9-e687a730bafb

---

### Setup

1. Clone the repository.

```sh
git clone https://github.com/defliez/matrix-screensaver.git
```

2. Compile the programs with the following commands:

```sh
gcc infinite-matrix.c -o matrix -lncurses
gcc infinite-matrix.c -o infinite-matrix -lncurses
```

3. Edit your hypridle config (or any other idle daemon) to run a command to launch the screensaver:

```
listener {
    timeout = 300
    on-timeout = pgrep -a ghostty | grep -q "matrix-screensaver" || ghostty --background-opacity=0.5 --title=matrix-screensaver -e /path/to/compiled-executable
}
```

4. Add the following line to your hyprland.conf file:

```
windowrulev2 = fullscreen,title:^(matrix-screensaver)$
```

5. Restart hypridle:

```sh
killall hypridle && hypridle & disown
```
