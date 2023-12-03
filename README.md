# L-System_Trees
## Generating 3D Models For Trees using L-Systems

This project used OpenGL and a Docker image to run it. 

##What does it do?
This project generates a tree starting with a simple string like: "X".
It then expands the string according to certain rules.
The rules look something like this:  X -> F&+[[X]+X]-F[+FX]+X. 

Finally, it transforms it into a system that closely simulates plant models and their growth.
Like the following:
<img src="./images/image1.png" width="400" height="480">

It can also simulate tree growth by incrementing the Lindenmayer System.
These images show the stages of a single tree as it incremented

<table>
    <tr>
        <td>
          <img src="./images/treeGrowth1.png" width="240" height="240">
        </td>
        <td>
          <img src="./images/treeGrowth2.png" width="240" height="240">
        </td>
    </tr>
    <tr>
        <td>
        <img src="./images/treeGrowth3.png" width="360" height="360">
        </td>
      <td>
        <img src="./images/treeGrowth4.png" width="360" height="360">
      </td>
  </tr>
</table>

## Here are the controls for the app:

- W: move camera up <br>
- S: move camera down <br>
- A: move camera left <br>
- D: move camera right <br>
- Q: rotate scene to the left <br>
- E: rotate scene to the right <br>
- Z: zoom camera out <br>
- X: zoom camera in <br>
- C: reset camera to original position <br>
- V: reset camera to downward view <br>
