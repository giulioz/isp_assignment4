# Assignment 4-CSF - Image Structuring Program

## Introduction

In Assignment 4 for computer science focused studies (A4-CSF) of the ISP practicals (KU), you will implement a command-line based image editor.
Please make sure that you follow the assignment description exactly, because even slight differences can cause test cases to fail.
This document describes the structure of the assignment and the expected functionality of the program.

## Learning Goals

- Heap and Memory Management
- File I/O
- Structs
- Enums

## Specifications

There are some general specifications that must be followed for every assignment in this course. A list of these and which point deductions may result from violating them can be found here:
[Assessment of Assignments - Deductions](https://www.notion.so/coding-tugraz/Assessment-of-Assignments-How-do-you-get-your-points-d7cca6cc89a344e38ad74dd3ccb73cb3?pvs=4#e3203e50b1fc4ba787bdbf5fb46262ae).
Please follow these specifications to avoid point deductions. As **memory management** is a central learning goal of this assignment, pay close attention to this section within the specifications.


Additionally, the following specifications must be followed for this assignment (A4-CSF) as well:

* **Attention:** `malloc()` (or `calloc()`), `realloc()` and `free()` must be used. Failure to do so may result in a deduction of **up to 25%**.
* **Attention:** Your program must be able to handle inputs of arbitrary length. Failure to do so may result in a deduction of **up to 25%**.
* **Attention:** All BMPs and layers must be stored on the heap. Failure to do so may result in a deduction of **up to 25%**.
* **Attention:** Your program must store the layers in a tree structure. The root of the tree represents the empty canvas and has the `LAYER_ID` 0. Failure to do so may result in a deduction of **up to 25%**.

## Grading

Assignment 4-CSF is worth **40 points**.

**Attention:** To receive a positive grade on the ISP KU, you must
1. pass at least 50% of the public testcases (This means that at least 5 public testcases have to be passed.) **and**
2. pass the assignment interview. (The assignment interview is considered passed if the interview-specific deductions for A4-CSF are less than 50%.)

Information on how your assignment submission will be assessed can be found here: [Assessment of Assignments](https://www.notion.so/coding-tugraz/Assessment-of-Assignments-How-do-you-get-your-points-d7cca6cc89a344e38ad74dd3ccb73cb3).
Please read this carefully, as it also contains guidelines on what your final code should look like (Coding Standard, Style Guide).

Here you can find an overview of how your total grade for the KU will be calculated: [Procedure and Grading](https://www.notion.so/coding-tugraz/Procedure-and-Grading-59975fee4a9c4047867772cd20caf73d)

## Submission

* Deliverables: `a4-csf.c`
* Push to your repository
* **Deadline: 21.01.2026, 18:00 (Austrian time)**

## Weekly Exercises

There are **Weekly Exercises** associated with this assignment available on [TeachCenter](https://tc.tugraz.at/main/course/section.php?id=72261).
These are simple exercises designed to help you prepare for the assignment, and **must** be solved at least partially to avoid deductions.

- **Attention**: At least 50% of the Weekly Exercises for A4-CSF must be solved before the A4-CSF deadline. Failure to meet this requirement will result in a **25% point deduction on A4-CSF**.

## Assignment Description

In this assignment, you will build a command-line image editor.
The editor works with BMP images, which you load into memory and place onto a canvas.
Every change you make becomes a layer in a tree structure, allowing you to undo actions, switch between previous states, and render the final image.

Your program will:

1. `Load` BMP files from disk and store them in a BMP library (in heap memory).
2. `Crop` parts of BMPs to create new BMPs.
3. `Place` a BMP onto the canvas, creating a layer.
4. Store all layers inside a tree:
  - The root (Layer 0) is just a white canvas.
  - Every `place` creates a child of the current active layer.
5. Support `undo` by moving to the parent layer.
6. Support `switch` to jump to any existing layer in the tree.
7. Render all layers from root to the selected layer into one final image.
8. `Print` this rendered image to the console using colored blocks.
9. `Save` the rendered image into a BMP file.
10. Free all memory before quitting.

### BMP Library

The **BMP library** is a collection of all BMP images that have been loaded from disk or created by cropping during program execution.
Each BMP in the library is stored in dynamically allocated memory and is identified by a unique `BMP_ID`.
- `BMP_ID`s are assigned sequentially, starting at 0.
- Every time a new BMP is added to the library (via `load` or `crop`), it receives the next available ID.
- If a command fails, the `BMP_ID` counter is not incremented.

BMPs in the library are never modified after creation. Even when cropping an image, the original BMP remains unchanged and a new BMP with its own `BMP_ID` is created.

These BMPs act as reusable building blocks that can be placed multiple times on the canvas.

### Canvas and Layers

The **canvas** is the working area where images are placed. It has a fixed width and height defined when the program starts. Initially, the canvas is empty and completely white.

A **layer** represents one editing step in the canvas history. Each time a BMP is placed on the canvas using the place command, a new layer is created. It does not store pixel data directly. Instead, each layer stores:
- which BMP to draw
- where to draw it on the canvas (x, y coordinates)
- which blend mode to use
- a link to its parent
- a list of children

This creates a tree of layers.

Layer IDs are assigned sequentially:
- The root layer (the empty canvas) always has `LAYER_ID` 0
- Each successful `place` command creates a new layer with the next available `LAYER_ID`
- Layer IDs are never reused, even if you undo or switch branches

### Layer Tree Structure

All layers are stored in a tree.

- The root of the tree (Layer 0) represents the empty canvas.
- Each time the user chooses to draw a new BMP, a new child layer is added to the current layer.
- All layers must be allocated on the heap.

This tree structure allows different editing paths to exist at the same time.

### General Notes

The notes given in this section are important for the entire assignment description:

- `\n` should not be printed as separate characters, instead they represent the newline character.
- Note the leading and trailing spaces in the text fields! You can make them visible by selecting the text field.
- Words in angle brackets (such as `<TEXT>`) should be replaced by calculated or given data. They are **not** part of the output itself! For a better understanding, you can look at the [example outputs](#example-outputs) at the very bottom of this document.
- <code>&nbsp;>&nbsp;</code> (command prompt, always printed with a leading and trailing space) in the output indicates that the program should wait for user input at this point. Execution should only continue after the user submits their input with `ENTER`.
- Make sure that your program can handle inputs of arbitrary length including empty inputs (see also the [specifications](#specifications)).
- If the user's input is invalid, the corresponding error message should be printed. After this, the same question is asked again, the command prompt is printed, and the user can input again. This process is repeated **as long as** the input is invalid.
- Any time the program waits for user input, `quit` should be a valid input. If the user enters `quit`, the program should terminate with the return value `0`. Take care to free all previously allocated memory!
- Leading and trailing spaces in user input are ignored.
- The user input (including commands) is **case-sensitive**, meaning `quit` is a valid input, but `QUIT` or `qUiT` are not.
- There must be at least one space between the command and its arguments, and between individual arguments. Additional spaces are permitted.
- Take care with your memory management! Whenever you allocate memory on the heap, you must check that the allocation was successful. If the program runs out of memory, you should free all memory previously allocated on the heap, print the correct error message and terminate the program with the corresponding return value (see [Return Values and Error Messages](#return-values-and-error-messages)).



## Structure of the Assignment

This assignment asks you to implement a simple command-line image editor.
The program allows users to load images, place them onto a canvas as layers, undo changes, switch between previous states, and render the final result.

The canvas is represented by a tree structure. The root of the tree represents the empty canvas and has the `LAYER_ID` 0. It should be rendered fully white.

- The `place` command adds a new layer to the tree. Each layer is assigned a unique `LAYER_ID` which can be used to switch the working branch to a specific layer.
- The `undo` command moves the active layer pointer one layer up in the tree.
- Using the `switch` command, the working branch can be switched to a specific layer. This allows the user to make many changes quickly and restore any previous state of their canvas.

To `print` the canvas to the console, the program has to traverse the tree for the specified layer and render the layers in the correct order, using the specified blend mode for each layer. All layers are allocated on the heap.

To give you a better understanding of the structure of the tree, here is an example of the tree after the following command sequence:

```
load a.bmp
load b.bmp
place 0 10 10 n
place 1 5 10 n
undo
undo
crop 0 1 1 10 10
place 2 1 1 s
undo
place 0 30 30 m
switch 3
place 1 10 20 n
undo
place 0 1 1 s
```

[//]: # (Ok this is maybe a solution that works in clion -gif but no mp4- and gitlab)
[//]: # (![Overview video]&#40;.internal/overview_structure.gif&#41;)
![Overview video](.internal/overview_structure.mp4)
<video src=".internal/overview_structure.mp4" poster=".internal/overview_structure.gif" style="max-width: 100%;">
</video>

### BMP Images

In this assignment, BMP images are used as input files because their structure is simple and predictable. If you would like to learn more about the structure of BMP images, please visit: [https://en.wikipedia.org/wiki/BMP_file_format#Example_2](https://en.wikipedia.org/wiki/BMP_file_format#Example_2))

You may assume that every valid BMP follows the exact format described below:

* All multi-byte values are stored in [little-endian format](https://en.wikipedia.org/wiki/Endianness).
* The first 2 bytes ALWAYS consist of `BM` ("Magic Number") - if this is not the case, the file is invalid.
* The pixel array start offset is stored at file position `0x0A`.
* The image width is stored at file position `0x12`.
* The image height is stored at file position `0x16`.
* Pixel data is stored in **BGRA order**:
  * Blue (1 byte)
  * Green (1 byte)
  * Red (1 byte)
  * Alpha (1 byte)
* Each pixel therefore occupies exactly 4 bytes.
* All images that are loaded will have exactly this layout (BITMAPV4HEADER with BI_BITFIELDS and BGRA layout) and are of no other type.

> **Attention:** The image content (the pixel colors) must be read from left to right and from **bottom to top**. The first pixel color value in the file therefore corresponds to the pixel at the bottom-left of the image.

#### File Structure

| Purpose | File Offset | Size in bytes      |
|-----------------|----------------------|--------------------|
| Magic Number `BM` | 0x00                    | 2                  |
| Pixel data offset | 0x0A                    | 4                  |
| Image width       | 0x12                    | 4                  |
| Image height      | 0x16                    | 4                  |
| Pixel array in BGRA format             | (see pixel data offset) | `height * width * 4` |


Example hex dump of a BMP file:

```
0x0000 42 4d 76 01 00 00 00 00   00 00 36 00 00 00 28 00
0x0010 00 00 0a 00 00 00 0a 00   00 00 01 00 18 00 00 00
0x0020 00 00 40 01 00 00 23 2e   00 00 23 2e 00 00 00 00
0x0030 00 00 00 00 00 00 bb 76   31 bb 76 31 bb 76 31 bb
0x0040 76 61 . . . . .
```

> **Note:** A hex dump shows the raw bytes of a file, where each pair of hexadecimal digits represents one byte and the value on the left (e.g. `0x0000`, `0x0010`) indicates the byte offset in the file. The offset values are not stored in the file itself. They are added by the hex dump tool to make the output easier to read and to show the position of each byte within the file.

* at 0x00: `42 4d` == Header (`BM`)
* at 0x0A: `36 00 00 00` == Start of pixel array (0x000036 = 54)
* at 0x12: `0a 00 00 00` == Width (0x00000a = 10)
* at 0x16: `0a 00 00 00` == Height (0x00000a = 10)
* at 0x36: `bb 76 31 00 bb 76 31 00 ...` == Pixel array

> **Note:** On typical Linux systems you can read these values without any problems; nothing needs to be reversed (except that they must be read from bottom
to top). t may appear as if the number is stored _backwards_, but as mentioned above, this is due to little-endian encoding, e.g., `36 00 00 00`
corresponds to `0x00000036`.

## Color Blending

When rendering the canvas, your program will combine several image layers into one final image. Think of it like stacking (semi-transparent) pictures on top of each other.
Each layer has a color for every pixel. To get the final image, the colors must be blended (mixed) using the supplied blend mode.
You always blend two layers at a time:

1. Start with the empty canvas (all white pixels with alpha = 255).
2. Blend Layer 1 onto it, the result becomes the new base.
3. Blend Layer 2 onto the result, again new base.
4. Continue until all layers are blended. (The result of each blend step always has alpha = 255.)

Blending always happens between two pixels at the same position:

- **A** — the pixel color that is already on the canvas (the background or lower layer)
- **B** — the pixel color from the layer currently being drawn (the foreground or top layer)
- **R** — the resulting pixel color after blending A and B
- **$\alpha$** — transparency of **B**

  Because alpha is stored as a value from 0–255, you must convert it to a value between 0 and 1 before using it: $\alpha = \frac{B_{\text{alpha}}}{255.}$

> **Note:** $\alpha$ must be stored as a `double` to match the expected test system output.

### Blend Modes

Your program must support three different blend modes.

> **Normal (**`n`**)**
>
> This is the most common blending mode. The top pixel partially replaces the bottom pixel depending on its alpha value.
> - Fully transparent: background remains unchanged
> - Fully opaque: top pixel fully replaces background
>
>> $R = \alpha \cdot B + (1 - \alpha) \cdot A$

> **Multiply (**`m`**)**
>
> This blend mode darkens the image by multiplying pixel values. The alpha value is ignored in this mode.
> - White pixels have no effect
> - Dark pixels darken the result
>> $R = (A \cdot B) / 255$

> **Sub (**`s`**)**
>
> This mode highlights differences between colors.
>> $R = \max(A, B) - \min(A, B)$

> **Note:** You need compute and save **R** for each color channel (red, green, blue).

## Program Sequence

The program is started by running:

```
./a4-csf <CANVAS_WIDTH> <CANVAS_HEIGHT>
```

where `<CANVAS_WIDTH>` and `<CANVAS_HEIGHT>` specify the size of the canvas in pixels.

If the number of command-line arguments is incorrect, or if either value is missing, non-numeric, or smaller than 1, the program must terminate immediately and print the appropriate error message. (see [Return Values and Error Messages](#return-values-and-error-messages))

<details><summary><h3>Welcome Message</h3></summary>

At the beginning of the program, the user is welcomed with the following message:

```
\n
Welcome to Image Structuring Program!\n
The canvas is <WIDTH> x <HEIGHT> pixels.\n
\n
```

</details>

<details><summary><h3>Command Loop</h3></summary>

After printing the welcome message, the program enters a loop in which it continuously reads commands from the console. This loop only terminates when the user enters `quit`. When quitting, the program must free all allocated memory and exit with the correct return value. (see [Return Values and Error Messages](#return-values-and-error-messages))

```
 > 
```

When the user enters a command, the program must first check whether the command is valid. It does this by verifying that the command exists and then checking whether the correct number of arguments was provided.

If any of these checks fail, the program prints the corresponding error message (see [Return Values and Error Messages](#return-values-and-error-messages)) and then re-prompts the user for a new command.

If all checks pass, the program attempts to execute the command. If an error occurs during execution, the appropriate error message must be printed (see [Return Values and Error Messages](#return-values-and-error-messages)) and the program must again re-prompt the user.

The user can enter one of the following commands:

<details><summary><h4>help</h4></summary>

The `help` command prints the following list of all available commands in the program:

```
\n
Available commands:\n
 help\n
 load <PATH>\n
 crop <BMP_ID> <TOP_X> <TOP_Y> <BOTTOM_X> <BOTTOM_Y>\n
 place <BMP_ID> <CANVAS_X> <CANVAS_Y> <BLEND_MODE>\n
 undo\n
 print\n
 switch <LAYER_ID>\n
 tree\n
 bmps\n
 save <FILE_PATH>\n
 quit\n
\n
```

**Syntax:** `help`

The `help` command does not take any parameters.

</details>

<details><summary><h4>load</h4></summary>

The `load` command loads a BMP file from the specified path into the BMP library. If the file is valid, the program allocates memory for the new BMP, reads the pixel data from the file, and assigns it a unique `BMP_ID` starting from 0.

On success, the following message must be printed:

```
Loaded <PATH> with ID <BMP_ID> and dimensions <BMP_WIDTH> <BMP_HEIGHT>\n
```

Where
- `<PATH>` is the file path exactly as provided by the user,
- `<BMP_ID>` is the unique identifier assigned to the newly loaded BMP, starting at 0,
- `<BMP_WIDTH>` is the width of the loaded BMP image in pixels, and
- `<BMP_HEIGHT>` is the height of the loaded BMP image in pixels.

**Syntax:** `load <PATH>`

The `load` command has one parameter:

- `<PATH>` - Path to the BMP file to be loaded.

The program must first check that the file exists and that it is a valid BMP file.
If the file is invalid, the program prints the appropriate error message and does not increment the `BMP_ID`. (see [Return Values and Error Messages](#return-values-and-error-messages))

> **Note:** You only need to validate the BMP file header. If the file starts with the correct BMP magic number, you may assume that the file is otherwise valid and follows the expected layout described in the [BMP Images](#bmp-images) section. No additional consistency or integrity checks are required.

**Example:**

_Assume two BMPs have already been loaded into the BMP library._

```
 > load path/to/file.bmp
Loaded path/to/file.bmp with ID 2\n
```

</details>

<details><summary><h4>crop</h4></summary>

The `crop` command creates a cropped version of an existing BMP and saves it as a new BMP in the library.

On success, the following message must be printed:

```
Cropped ID <BMP_ID> to new ID <NEW_ID> with dimensions <CROPPED_WIDTH> x <CROPPED_HEIGHT>\n
```

Where
- `<BMP_ID>` is the `BMP_ID` of the BMP to crop,
- `<NEW_ID>` is the `BMP_ID` assigned to the newly created cropped BMP, and
- `<CROPPED_WIDTH>` and `<CROPPED_HEIGHT>` are the dimensions of the cropped image in pixels.

**Syntax:** `crop <BMP_ID> <TOP_X> <TOP_Y> <BOTTOM_X> <BOTTOM_Y>`

The `crop` command has five parameters:

- `<BMP_ID>` - ID of the source BMP to crop.
- `<TOP_X> <TOP_Y>` - Coordinates of the top-left corner of the crop area.
- `<BOTTOM_X> <BOTTOM_Y>` - Coordinates of the bottom-right corner of the crop area.

If the specified `<BMP_ID>` exists and the given coordinates are valid, the program allocates memory for a new BMP and copies the pixel data from the specified rectangular area into it. The new BMP is assigned the next valid `BMP_ID`.

The crop area must satisfy all of the following:
- Both corners must be inside the source image.
- `<BOTTOM_X>` must not be smaller than `<TOP_X>`.
- `<BOTTOM_Y>` must not be smaller than `<TOP_Y>`.

If the specified `<BMP_ID>` is not found or the specified coordinates are invalid, the correct error message is printed. (see [Return Values and Error Messages](#return-values-and-error-messages))

**Examples:**

```
 > crop 0 10 10 20 20
Cropped ID 0 to new ID 1 with dimensions 11 x 11\n
```

The following examples show the result of applying different `crop` commands to the BMP displayed on the left-hand side:

![.internal/crop.png](.internal/crop.png)


</details>

<details><summary><h4>place</h4></summary>

The `place` command places a BMP from the BMP library onto the canvas as a new layer.

On success, the following message must be printed:

```
Switched to layer <LAYER_ID>\n
```

Where `<LAYER_ID>` is the ID of the newly created layer.

**Syntax:** `place <BMP_ID> <CANVAS_X> <CANVAS_Y> <BLEND_MODE>`

The `place` command has four parameters:

- `<BMP_ID>` - ID of the BMP to place on the canvas.
- `<CANVAS_X>` `<CANVAS_Y>` - Coordinates on the canvas where the top-left corner of the BMP will be placed.
- `<BLEND_MODE>` - Blend mode used to combine this layer with the layers below it.
  - `n` - normal blend mode
  - `m` - multiply blend mode
  - `s` - sub blend mode

If the specified `<BMP_ID>` exists and all parameters are valid, the program creates a new layer and adds it as a child of the currently active layer in the layer tree. The new layer becomes the active layer and is assigned a unique `LAYER_ID`.

> **Note:** The layer stores only the information required for rendering (BMP ID, canvas offset, and blend mode). No rendering is performed at this stage. Rendering is deferred until the canvas is printed.

The command fails if any of the following is true:
- `<BMP_ID>` does not exist
- The specified canvas coordinates lie outside the canvas
- The BMP does not fully fit on the canvas at the specified offset
- The blend mode is invalid

If the command fails, no layer is added and the appropriate error message is printed. (see [Return Values and Error Messages](#return-values-and-error-messages))

**Example:**

On a white canvas of size 8x8, the following command sequence:

```
 > load path/to/file.bmp
Loaded path/to/file.bmp with ID 0\n
 > crop 0 2 2 3 4
Cropped ID 0 to new ID 1 with dimensions 2 x 3\n
 > place 1 4 3 n
Switched to layer 1\n
 > print
```

places the cropped BMP with ID 1 at canvas position (4, 3) using the normal blend mode and makes the new layer active.

![.internal/place.png](.internal/place.png)

> **Note:** The canvas itself is rendered without a grid. The grid shown in the image above is provided solely for visualization and does not appear in the program output.

</details>

<details><summary><h4>undo</h4></summary>

The `undo` command moves the active layer to its parent in the layer tree.

**Syntax:** `undo`

The `undo` command does not take any parameters.

On success, the program should print the `LAYER_ID` of the active layer.

```
Switched to layer <LAYER_ID>\n
```

If the active layer is already the root of the tree (the canvas with `LAYER_ID` 0), the command fails and the program prints the appropriate error message. (see [Return Values and Error Messages](#return-values-and-error-messages))

</details>

<details><summary><h4>print</h4></summary>

The `print` command renders the currently selected layer and prints the resulting canvas to the console. The canvas is rendered by traversing the layer tree starting at the root layer (`LAYER_ID` 0) and rendering all layers along the path to the currently active layer. Layers must be rendered in order from root to leaf, applying the blend mode of each layer when combining it with the result so far.

Rendering always starts from the empty canvas that is fully white.

**Syntax:** `print`

The `print` command does not take any parameters.

**Canvas output format:**

After rendering, the resulting canvas is printed to the console using the following format rules:
- Each pixel is printed as three block characters: `███`
- The color of the pixel is set using ANSI escape sequences before printing the blocks.
- Column indices are printed at the top of the canvas:
  - First print three spaces
  - Then print each column index, padded to two digits and preceded by one space (width = 3 characters)
- Row indices are printed on the left of each row:
  - Each row starts with the row index padded to two digits
  - Followed by a `|` character
- The right side of the canvas is closed with a `|` character
- The bottom border consists of:
  - Two leading spaces
  - Followed by `width * 3 + 2` `-` characters

Example for a 2x3 canvas:

```
    01 02\n
01|██████|\n
02|██████|\n
03|██████|\n
  --------\n
```

Using the ANSI escape sequences, you can set the color of the pixel. \
For each pixel the following procedure needs to be done: \
Print a pixel with ```\033[38;2;<R>;<G>;<B>m███\033[0m``` where ```<R>```,```<G>```,```<B>``` are the red, green and blue values of the pixel. The block characters ```███``` are then printed in the previously specified color and with ```\033[0m``` the color is reset to the default.
Example of a pixel with red color:

```c
printf("\033[38;2;250;20;20m███\033[0m");
```

</details>
<details><summary><h4>switch</h4></summary>

The `switch` command switches the active layer to the specified layer in the layer tree.

On success, the program should print the `LAYER_ID` of the active layer.

```
Switched to layer <LAYER_ID>\n
```

Where `<LAYER_ID>` is the ID of the newly active layer.

**Syntax:** `switch <LAYER_ID>`

The `switch` command takes one parameter.

- `<LAYER_ID>` - ID of the layer that should become the active layer.

If the `<LAYER_ID>` does not exist, the command fails and the program prints the appropriate error message. (see [Return Values and Error Messages](#return-values-and-error-messages))

**Example:**

```
 > switch 1
Switched to layer 1\n
```

</details>

<details><summary><h4>tree</h4></summary>

The `tree` command prints an ASCII representation of the current layer tree. The tree is printed using a depth-first traversal, starting at the root layer (`LAYER_ID` 0).
For each layer, its children are traversed in the order they were added to their parent.

**Syntax:** `tree`

The `tree` command does not take any parameters.

**Output format:**

For each layer, one line is printed containing information about that layer.
- If the layer represents a placed BMP, the following format is used:
  ```
  Layer <LAYER_ID> renders BMP <BMP_ID> at <CANVAS_X> <CANVAS_Y>
  ```
  Where
  - `<LAYER_ID>` is the unique identifier of the layer being printed,
  - `<BMP_ID>` is  the ID of the BMP rendered by this layer, and
  - `<CANVAS_X>` and `<CANVAS_Y>` are the canvas coordinates of the top-left corner where the BMP is rendered.
- If the layer does not render a BMP (this only applies to the root layer), the following format is used:
  ```
  Layer <LAYER_ID>
  ```
  Where `<LAYER_ID>` is the unique identifier of the layer being printed.
- Each layer has leading spaces equal to ```depth × 3```, where depth is the depth of the layer in the tree (the root layer has depth 0).

**Example:**

For the following tree structure:

![.internal/tree.png](.internal/tree.png)

The output of the `tree` command would be:

```
Layer 0\n
   Layer 1 renders BMP 0 at 2 2\n
      Layer 2 renders BMP 0 at 1 1\n
      Layer 4 renders BMP 0 at 1 1\n
   Layer 3 renders BMP 0 at 2 6\n
```

> **Note:** In this example, each line describes a layer created by a previous `place` command. The values shown after `at` correspond to the `<CANVAS_X>` and `<CANVAS_Y>` coordinates that were specified when the BMP was placed on the canvas.

</details>

<details><summary><h4>bmps</h4></summary>

The `bmps` command prints an ASCII representation of the current BMP library.

**Syntax:** `bmps`

The `bmps` command does not take any parameters.

**Output format:**

For each BMP in the library, exactly one line is printed. The output is ordered by ascending `BMP_ID`:

```
BMP <BMP_ID> has dimensions <BMP_WIDTH> x <BMP_HEIGHT>\n
```
Where
- `<BMP_ID>` is the unique identifier of the BMP, and
- `<BMP_WIDTH>` and `<BMP_HEIGHT>` are the dimensions of the BMP.

**Example:**

For the following BMP library:

![.internal/bmp_library.png](.internal/bmp_library.png)

The output of the `bmps` command would be:

```
BMP 0 has dimensions 4 x 4\n
BMP 1 has dimensions 2 x 3\n
```

> **Note:** In this example, each line describes a BMP created by a previous `load` command.

</details>

<details><summary><h4>save</h4></summary>

The `save` command renders the currently active layer to the canvas and saves the resulting image to a BMP file.

On success, the following message should be printed:

```
Successfully saved image to <FILE_PATH>\n
```

Where `<FILE_PATH>` is the path provided by the user.

**Syntax:** `save <FILE_PATH>`

The `save` command takes one parameter.

- `<FILE_PATH>` - Path where the rendered canvas should be saved.

The canvas is rendered in the same way as the `print` command, starting from the root layer and applying all layers along the active branch in the correct order.

Unlike `print`, the rendered canvas is not displayed on the console. Instead, it is written to a BMP file at the specified file path.

A helper function for generating and filling the BMP header is provided and must be used when writing the output file.

```c
//----------------------------------------------------------------------------------------------------------------------
/// @brief Takes given BmpHeader pointer and fills in default values into that struct. 
/// @param header Pointer to the BmpHeader that needs to be filled
/// @param width Width of the bmp 
/// @param height Height of the bmp 
void fillBmpHeaderDefaultValues(BmpHeader* header, int width, int height);
```

This helper function is implemented in `bmp.c` and declared in `bmp.h`, which are provided as part of the assignment.

The function fills in all required default values of the BMP header to match the exact format expected by the test system.
You must not change any values of the BMP header after filling it with this function.
You need to write this header and then the missing color values at the correct location and with the correct layout to the file. (see [BMP File Structure](#file-structure))

If the specified file path is invalid or the file cannot be written, the command fails and the program prints the appropriate error message. (see [Return Values and Error Messages](#return-values-and-error-messages))

</details>

After each command execution, regardless of whether the command succeeds or fails, the program must display the prompt again and wait for the next command.

</details>

### Return Values and Error Messages

The program has to react to errors during the execution and print the corresponding error messages and/or return the correct return values. You can find all error messages that should be printed and all return values in the table below. Only one error should be printed at once. If multiple errors occur at the same time, only the highest message in the below table should be printed.

| Return Value | Error Message                                               | Meaning |
|--------------|-------------------------------------------------------------|---------|
| 0            | -                                                           | The program terminated successfully (user entered `quit`) |
| 1            | `[ERROR] Memory allocation failed!\n`                       | The program was not able to allocate new memory |
| 2            | `[ERROR] Invalid amount of command line parameters!\n`      | The program was not started with the correct number of command line arguments |
| 3            | `[ERROR] Invalid canvas size specified!\n`                  | The specified canvas dimensions are missing, non-numeric, or smaller than 1 |
|              | `[ERROR] Command unknown!\n`                                | The user entered an invalid command |
|              | `[ERROR] Wrong number of arguments!\n`                      | The command was called with too many or too few arguments |
|              | `[ERROR] BMP ID not found!\n`                               | [Crop & Place] The BMP_ID does not exist in the BMP library |
|              | `[ERROR] Cannot open file!\n`                               | [Load] The file could not be opened for reading |
|              | `[ERROR] Invalid file!\n`                                   | [Load] The file does not start with the correct magic number |
|              | `[ERROR] Crop coordinates are outside the BMP!\n`           | [Crop] At least one of the supplied crop coordinates is invalid or lies outside the dimensions of the specified BMP image |
|              | `[ERROR] Crop coordinates do not form a valid rectangle!\n` | [Crop] The second coordinate is either to the left or above the first coordinate |
|              | `[ERROR] Canvas coordinates are invalid!\n`                 | [Place] The specified `<CANVAS_X>` and `<CANVAS_Y>` coordinates are invalid or outside the boundaries of the canvas |
|              | `[ERROR] BMP does not fit on the canvas!\n`                 | [Place] The BMP would extend beyond the canvas boundaries at the given coordinates |
|              | `[ERROR] Invalid blend mode!\n`                             | [Place] The `<BLEND_MODE>` is not one of the allowed values |
|              | `[ERROR] Already at root layer!\n`                          | [Undo] The active layer is already the root layer |
|              | `[ERROR] Layer ID not found!\n`                             | [Switch] The specified `<LAYER_ID>` does not exist in the layer tree |
|              | `[ERROR] Invalid file path!\n`                              | [Save] The specified `<FILE_PATH>` is invalid or cannot be created. |


## Example Outputs
<details><summary><h4>Example 1</h4></summary>

```txt
Welcome to Image Structuring Program!
The canvas is 100 x 100 pixels.
 > load images/coding.bmp
Loaded images/coding.bmp with id 0 and dimensions 20 20
 > load images/coding_no_alpha.bmp
Loaded images/coding_no_alpha.bmp with id 1 and dimensions 50 50
 > load images/city.bmp
Loaded images/city.bmp with id 2 and dimensions 80 46
 > undo
[ERROR] Already at root layer!
 > place 0 10 10 s
Switched to layer 1
 > undo 
Switched to layer 0
 > place 0 10 10 s
Switched to layer 2
 > undo 
Switched to layer 0
 > place 0 10 10 s
Switched to layer 3
 > switch 1
Switched to layer 1
 > tree
Layer 0
   Layer 1 renders BMP 0 at 10 10
   Layer 2 renders BMP 0 at 10 10
   Layer 3 renders BMP 0 at 10 10
 > place 1 10 10 s
Switched to layer 4
 > undo 
Switched to layer 1
 > place 1 10 10 s
Switched to layer 5
 > tree
Layer 0
   Layer 1 renders BMP 0 at 10 10
      Layer 4 renders BMP 1 at 10 10
      Layer 5 renders BMP 1 at 10 10
   Layer 2 renders BMP 0 at 10 10
   Layer 3 renders BMP 0 at 10 10
 > switch 2
Switched to layer 2
 > place 1 10 10 s
Switched to layer 6
 > undo 
Switched to layer 2
 > place 1 10 10 s
Switched to layer 7
 > tree
Layer 0
   Layer 1 renders BMP 0 at 10 10
      Layer 4 renders BMP 1 at 10 10
      Layer 5 renders BMP 1 at 10 10
   Layer 2 renders BMP 0 at 10 10
      Layer 6 renders BMP 1 at 10 10
      Layer 7 renders BMP 1 at 10 10
   Layer 3 renders BMP 0 at 10 10
 > switch 3
Switched to layer 3
 > place 1 10 10 s
Switched to layer 8
 > undo 
Switched to layer 3
 > place 1 10 10 s
Switched to layer 9
 > tree
Layer 0
   Layer 1 renders BMP 0 at 10 10
      Layer 4 renders BMP 1 at 10 10
      Layer 5 renders BMP 1 at 10 10
   Layer 2 renders BMP 0 at 10 10
      Layer 6 renders BMP 1 at 10 10
      Layer 7 renders BMP 1 at 10 10
   Layer 3 renders BMP 0 at 10 10
      Layer 8 renders BMP 1 at 10 10
      Layer 9 renders BMP 1 at 10 10
 > quit

```
</details>

<details><summary><h4>Example 2</h4></summary>

```txt

Welcome to Image Structuring Program!
The canvas is 20 x 40 pixels.
 > load images/gradients.bmp
Loaded images/gradients.bmp with id 0 and dimensions 64 64
 > crop 0 1 1 20 40
Cropped 0 to new ID 1 with dimensions 20 x 40
 > crop 0 1 1 16 16
Cropped 0 to new ID 2 with dimensions 16 x 16
 > crop 0 17 1 32 16
Cropped 0 to new ID 3 with dimensions 16 x 16
 > place 1 1 1 n
Switched to layer 1
 > place 2 4 10 s
Switched to layer 2
 > place 3 1 20 m
Switched to layer 3
 > print
    01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20
01|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;254;254m███\x1b[0m\x1b[38;2;255;253;253m███\x1b[0m\x1b[38;2;255;252;252m███\x1b[0m\x1b[38;2;255;251;251m███\x1b[0m\x1b[38;2;255;250;250m███\x1b[0m\x1b[38;2;255;249;249m███\x1b[0m\x1b[38;2;255;248;248m███\x1b[0m\x1b[38;2;255;247;247m███\x1b[0m\x1b[38;2;255;246;246m███\x1b[0m\x1b[38;2;255;245;245m███\x1b[0m\x1b[38;2;255;244;244m███\x1b[0m\x1b[38;2;255;243;243m███\x1b[0m\x1b[38;2;255;242;242m███\x1b[0m\x1b[38;2;255;241;241m███\x1b[0m\x1b[38;2;255;240;240m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;254;255;254m███\x1b[0m\x1b[38;2;253;255;253m███\x1b[0m\x1b[38;2;252;255;252m███\x1b[0m|
02|\x1b[38;2;255;239;239m███\x1b[0m\x1b[38;2;255;238;238m███\x1b[0m\x1b[38;2;255;237;237m███\x1b[0m\x1b[38;2;255;236;236m███\x1b[0m\x1b[38;2;255;235;235m███\x1b[0m\x1b[38;2;255;234;234m███\x1b[0m\x1b[38;2;255;233;233m███\x1b[0m\x1b[38;2;255;232;232m███\x1b[0m\x1b[38;2;255;231;231m███\x1b[0m\x1b[38;2;255;230;230m███\x1b[0m\x1b[38;2;255;229;229m███\x1b[0m\x1b[38;2;255;228;228m███\x1b[0m\x1b[38;2;255;227;227m███\x1b[0m\x1b[38;2;255;226;226m███\x1b[0m\x1b[38;2;255;225;225m███\x1b[0m\x1b[38;2;255;224;224m███\x1b[0m\x1b[38;2;239;255;239m███\x1b[0m\x1b[38;2;238;255;238m███\x1b[0m\x1b[38;2;237;255;237m███\x1b[0m\x1b[38;2;236;255;236m███\x1b[0m|
03|\x1b[38;2;255;223;223m███\x1b[0m\x1b[38;2;255;222;222m███\x1b[0m\x1b[38;2;255;221;221m███\x1b[0m\x1b[38;2;255;220;220m███\x1b[0m\x1b[38;2;255;219;219m███\x1b[0m\x1b[38;2;255;218;218m███\x1b[0m\x1b[38;2;255;217;217m███\x1b[0m\x1b[38;2;255;216;216m███\x1b[0m\x1b[38;2;255;215;215m███\x1b[0m\x1b[38;2;255;214;214m███\x1b[0m\x1b[38;2;255;213;213m███\x1b[0m\x1b[38;2;254;211;211m███\x1b[0m\x1b[38;2;255;211;211m███\x1b[0m\x1b[38;2;255;210;210m███\x1b[0m\x1b[38;2;255;209;209m███\x1b[0m\x1b[38;2;255;208;208m███\x1b[0m\x1b[38;2;223;255;223m███\x1b[0m\x1b[38;2;222;255;222m███\x1b[0m\x1b[38;2;221;255;221m███\x1b[0m\x1b[38;2;220;255;220m███\x1b[0m|
04|\x1b[38;2;255;207;207m███\x1b[0m\x1b[38;2;255;206;206m███\x1b[0m\x1b[38;2;255;205;205m███\x1b[0m\x1b[38;2;255;204;204m███\x1b[0m\x1b[38;2;255;203;203m███\x1b[0m\x1b[38;2;255;202;202m███\x1b[0m\x1b[38;2;255;201;201m███\x1b[0m\x1b[38;2;255;200;200m███\x1b[0m\x1b[38;2;255;199;199m███\x1b[0m\x1b[38;2;255;198;198m███\x1b[0m\x1b[38;2;255;197;197m███\x1b[0m\x1b[38;2;254;195;195m███\x1b[0m\x1b[38;2;255;195;195m███\x1b[0m\x1b[38;2;255;194;194m███\x1b[0m\x1b[38;2;255;193;193m███\x1b[0m\x1b[38;2;255;192;192m███\x1b[0m\x1b[38;2;207;255;207m███\x1b[0m\x1b[38;2;206;255;206m███\x1b[0m\x1b[38;2;205;255;205m███\x1b[0m\x1b[38;2;204;255;204m███\x1b[0m|
05|\x1b[38;2;255;191;191m███\x1b[0m\x1b[38;2;255;190;190m███\x1b[0m\x1b[38;2;255;189;189m███\x1b[0m\x1b[38;2;255;188;188m███\x1b[0m\x1b[38;2;255;187;187m███\x1b[0m\x1b[38;2;255;186;186m███\x1b[0m\x1b[38;2;255;185;185m███\x1b[0m\x1b[38;2;255;184;184m███\x1b[0m\x1b[38;2;255;183;183m███\x1b[0m\x1b[38;2;255;182;182m███\x1b[0m\x1b[38;2;254;180;180m███\x1b[0m\x1b[38;2;254;179;179m███\x1b[0m\x1b[38;2;255;179;179m███\x1b[0m\x1b[38;2;255;178;178m███\x1b[0m\x1b[38;2;255;177;177m███\x1b[0m\x1b[38;2;255;176;176m███\x1b[0m\x1b[38;2;191;255;191m███\x1b[0m\x1b[38;2;190;255;190m███\x1b[0m\x1b[38;2;189;255;189m███\x1b[0m\x1b[38;2;188;255;188m███\x1b[0m|
06|\x1b[38;2;255;175;175m███\x1b[0m\x1b[38;2;255;174;174m███\x1b[0m\x1b[38;2;255;173;173m███\x1b[0m\x1b[38;2;255;172;172m███\x1b[0m\x1b[38;2;255;171;171m███\x1b[0m\x1b[38;2;255;170;170m███\x1b[0m\x1b[38;2;255;169;169m███\x1b[0m\x1b[38;2;255;168;168m███\x1b[0m\x1b[38;2;255;167;167m███\x1b[0m\x1b[38;2;255;166;166m███\x1b[0m\x1b[38;2;254;164;164m███\x1b[0m\x1b[38;2;254;163;163m███\x1b[0m\x1b[38;2;255;163;163m███\x1b[0m\x1b[38;2;255;162;162m███\x1b[0m\x1b[38;2;255;161;161m███\x1b[0m\x1b[38;2;255;160;160m███\x1b[0m\x1b[38;2;175;255;175m███\x1b[0m\x1b[38;2;174;255;174m███\x1b[0m\x1b[38;2;173;255;173m███\x1b[0m\x1b[38;2;172;255;172m███\x1b[0m|
07|\x1b[38;2;255;159;159m███\x1b[0m\x1b[38;2;255;158;158m███\x1b[0m\x1b[38;2;255;157;157m███\x1b[0m\x1b[38;2;255;156;156m███\x1b[0m\x1b[38;2;255;155;155m███\x1b[0m\x1b[38;2;255;154;154m███\x1b[0m\x1b[38;2;255;153;153m███\x1b[0m\x1b[38;2;255;152;152m███\x1b[0m\x1b[38;2;255;151;151m███\x1b[0m\x1b[38;2;255;150;150m███\x1b[0m\x1b[38;2;254;148;148m███\x1b[0m\x1b[38;2;254;147;147m███\x1b[0m\x1b[38;2;255;147;147m███\x1b[0m\x1b[38;2;255;146;146m███\x1b[0m\x1b[38;2;255;145;145m███\x1b[0m\x1b[38;2;255;144;144m███\x1b[0m\x1b[38;2;159;255;159m███\x1b[0m\x1b[38;2;158;255;158m███\x1b[0m\x1b[38;2;157;255;157m███\x1b[0m\x1b[38;2;156;255;156m███\x1b[0m|
08|\x1b[38;2;255;143;143m███\x1b[0m\x1b[38;2;255;142;142m███\x1b[0m\x1b[38;2;255;141;141m███\x1b[0m\x1b[38;2;255;140;140m███\x1b[0m\x1b[38;2;255;139;139m███\x1b[0m\x1b[38;2;255;138;138m███\x1b[0m\x1b[38;2;255;137;137m███\x1b[0m\x1b[38;2;255;136;136m███\x1b[0m\x1b[38;2;255;135;135m███\x1b[0m\x1b[38;2;255;134;134m███\x1b[0m\x1b[38;2;254;132;132m███\x1b[0m\x1b[38;2;254;131;131m███\x1b[0m\x1b[38;2;255;131;131m███\x1b[0m\x1b[38;2;255;130;130m███\x1b[0m\x1b[38;2;255;129;129m███\x1b[0m\x1b[38;2;255;128;128m███\x1b[0m\x1b[38;2;143;255;143m███\x1b[0m\x1b[38;2;142;255;142m███\x1b[0m\x1b[38;2;141;255;141m███\x1b[0m\x1b[38;2;140;255;140m███\x1b[0m|
09|\x1b[38;2;255;127;127m███\x1b[0m\x1b[38;2;255;126;126m███\x1b[0m\x1b[38;2;255;125;125m███\x1b[0m\x1b[38;2;255;124;124m███\x1b[0m\x1b[38;2;255;122;122m███\x1b[0m\x1b[38;2;255;121;121m███\x1b[0m\x1b[38;2;255;121;121m███\x1b[0m\x1b[38;2;255;120;120m███\x1b[0m\x1b[38;2;255;119;119m███\x1b[0m\x1b[38;2;255;118;118m███\x1b[0m\x1b[38;2;255;117;117m███\x1b[0m\x1b[38;2;255;116;116m███\x1b[0m\x1b[38;2;255;114;114m███\x1b[0m\x1b[38;2;255;113;113m███\x1b[0m\x1b[38;2;255;113;113m███\x1b[0m\x1b[38;2;255;112;112m███\x1b[0m\x1b[38;2;127;255;127m███\x1b[0m\x1b[38;2;126;255;126m███\x1b[0m\x1b[38;2;125;255;125m███\x1b[0m\x1b[38;2;124;255;124m███\x1b[0m|
10|\x1b[38;2;255;111;111m███\x1b[0m\x1b[38;2;255;110;110m███\x1b[0m\x1b[38;2;255;109;109m███\x1b[0m\x1b[38;2;0;108;108m███\x1b[0m\x1b[38;2;0;106;106m███\x1b[0m\x1b[38;2;0;105;105m███\x1b[0m\x1b[38;2;0;105;105m███\x1b[0m\x1b[38;2;0;104;104m███\x1b[0m\x1b[38;2;0;103;103m███\x1b[0m\x1b[38;2;0;102;102m███\x1b[0m\x1b[38;2;0;101;101m███\x1b[0m\x1b[38;2;0;100;100m███\x1b[0m\x1b[38;2;0;98;98m███\x1b[0m\x1b[38;2;0;97;97m███\x1b[0m\x1b[38;2;0;97;97m███\x1b[0m\x1b[38;2;0;96;96m███\x1b[0m\x1b[38;2;144;255;111m███\x1b[0m\x1b[38;2;145;255;110m███\x1b[0m\x1b[38;2;146;255;109m███\x1b[0m\x1b[38;2;108;255;108m███\x1b[0m|
11|\x1b[38;2;255;95;95m███\x1b[0m\x1b[38;2;255;94;94m███\x1b[0m\x1b[38;2;255;93;93m███\x1b[0m\x1b[38;2;0;92;92m███\x1b[0m\x1b[38;2;0;90;90m███\x1b[0m\x1b[38;2;0;89;89m███\x1b[0m\x1b[38;2;0;89;89m███\x1b[0m\x1b[38;2;0;88;88m███\x1b[0m\x1b[38;2;0;87;87m███\x1b[0m\x1b[38;2;0;86;86m███\x1b[0m\x1b[38;2;0;85;85m███\x1b[0m\x1b[38;2;0;84;84m███\x1b[0m\x1b[38;2;0;82;82m███\x1b[0m\x1b[38;2;0;81;81m███\x1b[0m\x1b[38;2;0;81;81m███\x1b[0m\x1b[38;2;0;80;80m███\x1b[0m\x1b[38;2;160;255;95m███\x1b[0m\x1b[38;2;161;255;94m███\x1b[0m\x1b[38;2;162;255;93m███\x1b[0m\x1b[38;2;92;255;92m███\x1b[0m|
12|\x1b[38;2;255;79;79m███\x1b[0m\x1b[38;2;255;78;78m███\x1b[0m\x1b[38;2;255;77;77m███\x1b[0m\x1b[38;2;0;76;76m███\x1b[0m\x1b[38;2;0;74;74m███\x1b[0m\x1b[38;2;0;73;73m███\x1b[0m\x1b[38;2;0;73;73m███\x1b[0m\x1b[38;2;0;72;72m███\x1b[0m\x1b[38;2;0;71;71m███\x1b[0m\x1b[38;2;0;70;70m███\x1b[0m\x1b[38;2;0;69;69m███\x1b[0m\x1b[38;2;0;68;68m███\x1b[0m\x1b[38;2;0;66;66m███\x1b[0m\x1b[38;2;0;65;65m███\x1b[0m\x1b[38;2;0;65;65m███\x1b[0m\x1b[38;2;0;64;64m███\x1b[0m\x1b[38;2;176;255;79m███\x1b[0m\x1b[38;2;177;255;78m███\x1b[0m\x1b[38;2;178;255;77m███\x1b[0m\x1b[38;2;76;255;76m███\x1b[0m|
13|\x1b[38;2;255;63;63m███\x1b[0m\x1b[38;2;255;62;62m███\x1b[0m\x1b[38;2;255;61;61m███\x1b[0m\x1b[38;2;0;60;60m███\x1b[0m\x1b[38;2;0;58;58m███\x1b[0m\x1b[38;2;0;57;57m███\x1b[0m\x1b[38;2;0;56;56m███\x1b[0m\x1b[38;2;0;56;56m███\x1b[0m\x1b[38;2;0;55;55m███\x1b[0m\x1b[38;2;0;54;54m███\x1b[0m\x1b[38;2;0;53;53m███\x1b[0m\x1b[38;2;0;52;52m███\x1b[0m\x1b[38;2;0;50;50m███\x1b[0m\x1b[38;2;0;49;49m███\x1b[0m\x1b[38;2;0;48;48m███\x1b[0m\x1b[38;2;0;48;48m███\x1b[0m\x1b[38;2;192;255;63m███\x1b[0m\x1b[38;2;193;255;62m███\x1b[0m\x1b[38;2;194;255;61m███\x1b[0m\x1b[38;2;60;255;60m███\x1b[0m|
14|\x1b[38;2;255;47;47m███\x1b[0m\x1b[38;2;255;46;46m███\x1b[0m\x1b[38;2;255;45;45m███\x1b[0m\x1b[38;2;0;44;44m███\x1b[0m\x1b[38;2;0;42;42m███\x1b[0m\x1b[38;2;0;41;41m███\x1b[0m\x1b[38;2;0;40;40m███\x1b[0m\x1b[38;2;0;40;40m███\x1b[0m\x1b[38;2;0;39;39m███\x1b[0m\x1b[38;2;0;38;38m███\x1b[0m\x1b[38;2;0;37;37m███\x1b[0m\x1b[38;2;0;36;36m███\x1b[0m\x1b[38;2;0;34;34m███\x1b[0m\x1b[38;2;0;33;33m███\x1b[0m\x1b[38;2;0;32;32m███\x1b[0m\x1b[38;2;0;32;32m███\x1b[0m\x1b[38;2;208;255;47m███\x1b[0m\x1b[38;2;209;255;46m███\x1b[0m\x1b[38;2;210;255;45m███\x1b[0m\x1b[38;2;44;255;44m███\x1b[0m|
15|\x1b[38;2;255;31;31m███\x1b[0m\x1b[38;2;255;30;30m███\x1b[0m\x1b[38;2;255;29;29m███\x1b[0m\x1b[38;2;0;28;28m███\x1b[0m\x1b[38;2;0;26;26m███\x1b[0m\x1b[38;2;0;25;25m███\x1b[0m\x1b[38;2;0;24;24m███\x1b[0m\x1b[38;2;0;24;24m███\x1b[0m\x1b[38;2;0;23;23m███\x1b[0m\x1b[38;2;0;22;22m███\x1b[0m\x1b[38;2;0;21;21m███\x1b[0m\x1b[38;2;0;20;20m███\x1b[0m\x1b[38;2;0;18;18m███\x1b[0m\x1b[38;2;0;17;17m███\x1b[0m\x1b[38;2;0;16;16m███\x1b[0m\x1b[38;2;0;16;16m███\x1b[0m\x1b[38;2;224;255;31m███\x1b[0m\x1b[38;2;225;255;30m███\x1b[0m\x1b[38;2;226;255;29m███\x1b[0m\x1b[38;2;28;255;28m███\x1b[0m|
16|\x1b[38;2;255;15;15m███\x1b[0m\x1b[38;2;255;14;14m███\x1b[0m\x1b[38;2;255;13;13m███\x1b[0m\x1b[38;2;0;12;12m███\x1b[0m\x1b[38;2;0;10;10m███\x1b[0m\x1b[38;2;0;9;9m███\x1b[0m\x1b[38;2;0;8;8m███\x1b[0m\x1b[38;2;0;8;8m███\x1b[0m\x1b[38;2;0;7;7m███\x1b[0m\x1b[38;2;0;6;6m███\x1b[0m\x1b[38;2;0;5;5m███\x1b[0m\x1b[38;2;0;4;4m███\x1b[0m\x1b[38;2;0;2;2m███\x1b[0m\x1b[38;2;0;1;1m███\x1b[0m\x1b[38;2;0;0;0m███\x1b[0m\x1b[38;2;0;0;0m███\x1b[0m\x1b[38;2;240;255;15m███\x1b[0m\x1b[38;2;241;255;14m███\x1b[0m\x1b[38;2;242;255;13m███\x1b[0m\x1b[38;2;12;255;12m███\x1b[0m|
17|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;254;255m███\x1b[0m\x1b[38;2;255;253;255m███\x1b[0m\x1b[38;2;0;252;255m███\x1b[0m\x1b[38;2;0;251;255m███\x1b[0m\x1b[38;2;0;250;255m███\x1b[0m\x1b[38;2;0;249;255m███\x1b[0m\x1b[38;2;0;248;255m███\x1b[0m\x1b[38;2;0;247;255m███\x1b[0m\x1b[38;2;0;246;255m███\x1b[0m\x1b[38;2;0;245;255m███\x1b[0m\x1b[38;2;0;244;255m███\x1b[0m\x1b[38;2;0;243;255m███\x1b[0m\x1b[38;2;0;242;255m███\x1b[0m\x1b[38;2;0;241;255m███\x1b[0m\x1b[38;2;0;240;255m███\x1b[0m\x1b[38;2;0;255;255m███\x1b[0m\x1b[38;2;1;255;255m███\x1b[0m\x1b[38;2;2;255;255m███\x1b[0m\x1b[38;2;252;255;255m███\x1b[0m|
18|\x1b[38;2;255;239;255m███\x1b[0m\x1b[38;2;255;238;255m███\x1b[0m\x1b[38;2;255;237;255m███\x1b[0m\x1b[38;2;0;236;255m███\x1b[0m\x1b[38;2;0;235;255m███\x1b[0m\x1b[38;2;0;234;255m███\x1b[0m\x1b[38;2;0;233;255m███\x1b[0m\x1b[38;2;0;232;255m███\x1b[0m\x1b[38;2;0;231;255m███\x1b[0m\x1b[38;2;0;230;255m███\x1b[0m\x1b[38;2;0;229;255m███\x1b[0m\x1b[38;2;0;228;255m███\x1b[0m\x1b[38;2;0;227;255m███\x1b[0m\x1b[38;2;0;226;255m███\x1b[0m\x1b[38;2;0;225;255m███\x1b[0m\x1b[38;2;0;224;255m███\x1b[0m\x1b[38;2;16;255;255m███\x1b[0m\x1b[38;2;17;255;255m███\x1b[0m\x1b[38;2;18;255;255m███\x1b[0m\x1b[38;2;236;255;255m███\x1b[0m|
19|\x1b[38;2;255;223;255m███\x1b[0m\x1b[38;2;255;222;255m███\x1b[0m\x1b[38;2;255;221;255m███\x1b[0m\x1b[38;2;0;220;255m███\x1b[0m\x1b[38;2;0;219;255m███\x1b[0m\x1b[38;2;0;218;255m███\x1b[0m\x1b[38;2;0;217;255m███\x1b[0m\x1b[38;2;0;216;255m███\x1b[0m\x1b[38;2;0;215;255m███\x1b[0m\x1b[38;2;0;214;255m███\x1b[0m\x1b[38;2;0;213;255m███\x1b[0m\x1b[38;2;1;211;254m███\x1b[0m\x1b[38;2;0;211;255m███\x1b[0m\x1b[38;2;0;210;255m███\x1b[0m\x1b[38;2;0;209;255m███\x1b[0m\x1b[38;2;0;208;255m███\x1b[0m\x1b[38;2;32;255;255m███\x1b[0m\x1b[38;2;33;255;255m███\x1b[0m\x1b[38;2;34;255;255m███\x1b[0m\x1b[38;2;220;255;255m███\x1b[0m|
20|\x1b[38;2;0;207;0m███\x1b[0m\x1b[38;2;0;206;0m███\x1b[0m\x1b[38;2;0;205;0m███\x1b[0m\x1b[38;2;0;204;0m███\x1b[0m\x1b[38;2;0;203;0m███\x1b[0m\x1b[38;2;0;202;0m███\x1b[0m\x1b[38;2;0;201;0m███\x1b[0m\x1b[38;2;0;200;0m███\x1b[0m\x1b[38;2;0;199;0m███\x1b[0m\x1b[38;2;0;198;0m███\x1b[0m\x1b[38;2;0;197;0m███\x1b[0m\x1b[38;2;0;195;0m███\x1b[0m\x1b[38;2;0;195;0m███\x1b[0m\x1b[38;2;0;194;0m███\x1b[0m\x1b[38;2;0;193;0m███\x1b[0m\x1b[38;2;0;192;0m███\x1b[0m\x1b[38;2;48;255;255m███\x1b[0m\x1b[38;2;49;255;255m███\x1b[0m\x1b[38;2;50;255;255m███\x1b[0m\x1b[38;2;204;255;255m███\x1b[0m|
21|\x1b[38;2;0;191;0m███\x1b[0m\x1b[38;2;0;190;0m███\x1b[0m\x1b[38;2;0;189;0m███\x1b[0m\x1b[38;2;0;188;0m███\x1b[0m\x1b[38;2;0;187;0m███\x1b[0m\x1b[38;2;0;186;0m███\x1b[0m\x1b[38;2;0;185;0m███\x1b[0m\x1b[38;2;0;184;0m███\x1b[0m\x1b[38;2;0;183;0m███\x1b[0m\x1b[38;2;0;182;0m███\x1b[0m\x1b[38;2;0;180;0m███\x1b[0m\x1b[38;2;0;179;0m███\x1b[0m\x1b[38;2;0;179;0m███\x1b[0m\x1b[38;2;0;178;0m███\x1b[0m\x1b[38;2;0;177;0m███\x1b[0m\x1b[38;2;0;176;0m███\x1b[0m\x1b[38;2;64;255;255m███\x1b[0m\x1b[38;2;65;255;255m███\x1b[0m\x1b[38;2;66;255;255m███\x1b[0m\x1b[38;2;188;255;255m███\x1b[0m|
22|\x1b[38;2;0;175;0m███\x1b[0m\x1b[38;2;0;174;0m███\x1b[0m\x1b[38;2;0;173;0m███\x1b[0m\x1b[38;2;0;172;0m███\x1b[0m\x1b[38;2;0;171;0m███\x1b[0m\x1b[38;2;0;170;0m███\x1b[0m\x1b[38;2;0;169;0m███\x1b[0m\x1b[38;2;0;168;0m███\x1b[0m\x1b[38;2;0;167;0m███\x1b[0m\x1b[38;2;0;166;0m███\x1b[0m\x1b[38;2;0;164;0m███\x1b[0m\x1b[38;2;0;163;0m███\x1b[0m\x1b[38;2;0;163;0m███\x1b[0m\x1b[38;2;0;162;0m███\x1b[0m\x1b[38;2;0;161;0m███\x1b[0m\x1b[38;2;0;160;0m███\x1b[0m\x1b[38;2;80;255;255m███\x1b[0m\x1b[38;2;81;255;255m███\x1b[0m\x1b[38;2;82;255;255m███\x1b[0m\x1b[38;2;172;255;255m███\x1b[0m|
23|\x1b[38;2;0;159;0m███\x1b[0m\x1b[38;2;0;158;0m███\x1b[0m\x1b[38;2;0;157;0m███\x1b[0m\x1b[38;2;0;156;0m███\x1b[0m\x1b[38;2;0;155;0m███\x1b[0m\x1b[38;2;0;154;0m███\x1b[0m\x1b[38;2;0;153;0m███\x1b[0m\x1b[38;2;0;152;0m███\x1b[0m\x1b[38;2;0;151;0m███\x1b[0m\x1b[38;2;0;150;0m███\x1b[0m\x1b[38;2;0;148;0m███\x1b[0m\x1b[38;2;0;147;0m███\x1b[0m\x1b[38;2;0;147;0m███\x1b[0m\x1b[38;2;0;146;0m███\x1b[0m\x1b[38;2;0;145;0m███\x1b[0m\x1b[38;2;0;144;0m███\x1b[0m\x1b[38;2;96;255;255m███\x1b[0m\x1b[38;2;97;255;255m███\x1b[0m\x1b[38;2;98;255;255m███\x1b[0m\x1b[38;2;156;255;255m███\x1b[0m|
24|\x1b[38;2;0;143;0m███\x1b[0m\x1b[38;2;0;142;0m███\x1b[0m\x1b[38;2;0;141;0m███\x1b[0m\x1b[38;2;0;140;0m███\x1b[0m\x1b[38;2;0;139;0m███\x1b[0m\x1b[38;2;0;138;0m███\x1b[0m\x1b[38;2;0;137;0m███\x1b[0m\x1b[38;2;0;136;0m███\x1b[0m\x1b[38;2;0;135;0m███\x1b[0m\x1b[38;2;0;134;0m███\x1b[0m\x1b[38;2;0;132;0m███\x1b[0m\x1b[38;2;0;131;0m███\x1b[0m\x1b[38;2;0;131;0m███\x1b[0m\x1b[38;2;0;130;0m███\x1b[0m\x1b[38;2;0;129;0m███\x1b[0m\x1b[38;2;0;128;0m███\x1b[0m\x1b[38;2;112;255;255m███\x1b[0m\x1b[38;2;113;255;255m███\x1b[0m\x1b[38;2;114;255;255m███\x1b[0m\x1b[38;2;140;255;255m███\x1b[0m|
25|\x1b[38;2;0;127;0m███\x1b[0m\x1b[38;2;0;126;0m███\x1b[0m\x1b[38;2;0;125;0m███\x1b[0m\x1b[38;2;0;124;0m███\x1b[0m\x1b[38;2;0;122;0m███\x1b[0m\x1b[38;2;0;121;0m███\x1b[0m\x1b[38;2;0;121;0m███\x1b[0m\x1b[38;2;0;120;0m███\x1b[0m\x1b[38;2;0;119;0m███\x1b[0m\x1b[38;2;0;118;0m███\x1b[0m\x1b[38;2;0;117;0m███\x1b[0m\x1b[38;2;0;116;0m███\x1b[0m\x1b[38;2;0;114;0m███\x1b[0m\x1b[38;2;0;113;0m███\x1b[0m\x1b[38;2;0;113;0m███\x1b[0m\x1b[38;2;0;112;0m███\x1b[0m\x1b[38;2;128;255;255m███\x1b[0m\x1b[38;2;129;255;255m███\x1b[0m\x1b[38;2;130;255;255m███\x1b[0m\x1b[38;2;124;255;255m███\x1b[0m|
26|\x1b[38;2;0;111;0m███\x1b[0m\x1b[38;2;0;110;0m███\x1b[0m\x1b[38;2;0;109;0m███\x1b[0m\x1b[38;2;0;108;0m███\x1b[0m\x1b[38;2;0;106;0m███\x1b[0m\x1b[38;2;0;105;0m███\x1b[0m\x1b[38;2;0;105;0m███\x1b[0m\x1b[38;2;0;104;0m███\x1b[0m\x1b[38;2;0;103;0m███\x1b[0m\x1b[38;2;0;102;0m███\x1b[0m\x1b[38;2;0;101;0m███\x1b[0m\x1b[38;2;0;100;0m███\x1b[0m\x1b[38;2;0;98;0m███\x1b[0m\x1b[38;2;0;97;0m███\x1b[0m\x1b[38;2;0;97;0m███\x1b[0m\x1b[38;2;0;96;0m███\x1b[0m\x1b[38;2;111;255;255m███\x1b[0m\x1b[38;2;110;255;255m███\x1b[0m\x1b[38;2;109;255;255m███\x1b[0m\x1b[38;2;108;255;255m███\x1b[0m|
27|\x1b[38;2;0;95;0m███\x1b[0m\x1b[38;2;0;94;0m███\x1b[0m\x1b[38;2;0;93;0m███\x1b[0m\x1b[38;2;0;92;0m███\x1b[0m\x1b[38;2;0;90;0m███\x1b[0m\x1b[38;2;0;89;0m███\x1b[0m\x1b[38;2;0;89;0m███\x1b[0m\x1b[38;2;0;88;0m███\x1b[0m\x1b[38;2;0;87;0m███\x1b[0m\x1b[38;2;0;86;0m███\x1b[0m\x1b[38;2;0;85;0m███\x1b[0m\x1b[38;2;0;84;0m███\x1b[0m\x1b[38;2;0;82;0m███\x1b[0m\x1b[38;2;0;81;0m███\x1b[0m\x1b[38;2;0;81;0m███\x1b[0m\x1b[38;2;0;80;0m███\x1b[0m\x1b[38;2;95;255;255m███\x1b[0m\x1b[38;2;94;255;255m███\x1b[0m\x1b[38;2;93;255;255m███\x1b[0m\x1b[38;2;92;255;255m███\x1b[0m|
28|\x1b[38;2;0;79;0m███\x1b[0m\x1b[38;2;0;78;0m███\x1b[0m\x1b[38;2;0;77;0m███\x1b[0m\x1b[38;2;0;76;0m███\x1b[0m\x1b[38;2;0;74;0m███\x1b[0m\x1b[38;2;0;73;0m███\x1b[0m\x1b[38;2;0;73;0m███\x1b[0m\x1b[38;2;0;72;0m███\x1b[0m\x1b[38;2;0;71;0m███\x1b[0m\x1b[38;2;0;70;0m███\x1b[0m\x1b[38;2;0;69;0m███\x1b[0m\x1b[38;2;0;68;0m███\x1b[0m\x1b[38;2;0;66;0m███\x1b[0m\x1b[38;2;0;65;0m███\x1b[0m\x1b[38;2;0;65;0m███\x1b[0m\x1b[38;2;0;64;0m███\x1b[0m\x1b[38;2;79;255;255m███\x1b[0m\x1b[38;2;78;255;255m███\x1b[0m\x1b[38;2;77;255;255m███\x1b[0m\x1b[38;2;76;255;255m███\x1b[0m|
29|\x1b[38;2;0;63;0m███\x1b[0m\x1b[38;2;0;62;0m███\x1b[0m\x1b[38;2;0;61;0m███\x1b[0m\x1b[38;2;0;60;0m███\x1b[0m\x1b[38;2;0;58;0m███\x1b[0m\x1b[38;2;0;57;0m███\x1b[0m\x1b[38;2;0;56;0m███\x1b[0m\x1b[38;2;0;56;0m███\x1b[0m\x1b[38;2;0;55;0m███\x1b[0m\x1b[38;2;0;54;0m███\x1b[0m\x1b[38;2;0;53;0m███\x1b[0m\x1b[38;2;0;52;0m███\x1b[0m\x1b[38;2;0;50;0m███\x1b[0m\x1b[38;2;0;49;0m███\x1b[0m\x1b[38;2;0;48;0m███\x1b[0m\x1b[38;2;0;48;0m███\x1b[0m\x1b[38;2;63;255;255m███\x1b[0m\x1b[38;2;62;255;255m███\x1b[0m\x1b[38;2;61;255;255m███\x1b[0m\x1b[38;2;60;255;255m███\x1b[0m|
30|\x1b[38;2;0;47;0m███\x1b[0m\x1b[38;2;0;46;0m███\x1b[0m\x1b[38;2;0;45;0m███\x1b[0m\x1b[38;2;0;44;0m███\x1b[0m\x1b[38;2;0;42;0m███\x1b[0m\x1b[38;2;0;41;0m███\x1b[0m\x1b[38;2;0;40;0m███\x1b[0m\x1b[38;2;0;40;0m███\x1b[0m\x1b[38;2;0;39;0m███\x1b[0m\x1b[38;2;0;38;0m███\x1b[0m\x1b[38;2;0;37;0m███\x1b[0m\x1b[38;2;0;36;0m███\x1b[0m\x1b[38;2;0;34;0m███\x1b[0m\x1b[38;2;0;33;0m███\x1b[0m\x1b[38;2;0;32;0m███\x1b[0m\x1b[38;2;0;32;0m███\x1b[0m\x1b[38;2;47;255;255m███\x1b[0m\x1b[38;2;46;255;255m███\x1b[0m\x1b[38;2;45;255;255m███\x1b[0m\x1b[38;2;44;255;255m███\x1b[0m|
31|\x1b[38;2;0;31;0m███\x1b[0m\x1b[38;2;0;30;0m███\x1b[0m\x1b[38;2;0;29;0m███\x1b[0m\x1b[38;2;0;28;0m███\x1b[0m\x1b[38;2;0;26;0m███\x1b[0m\x1b[38;2;0;25;0m███\x1b[0m\x1b[38;2;0;24;0m███\x1b[0m\x1b[38;2;0;24;0m███\x1b[0m\x1b[38;2;0;23;0m███\x1b[0m\x1b[38;2;0;22;0m███\x1b[0m\x1b[38;2;0;21;0m███\x1b[0m\x1b[38;2;0;20;0m███\x1b[0m\x1b[38;2;0;18;0m███\x1b[0m\x1b[38;2;0;17;0m███\x1b[0m\x1b[38;2;0;16;0m███\x1b[0m\x1b[38;2;0;16;0m███\x1b[0m\x1b[38;2;31;255;255m███\x1b[0m\x1b[38;2;30;255;255m███\x1b[0m\x1b[38;2;29;255;255m███\x1b[0m\x1b[38;2;28;255;255m███\x1b[0m|
32|\x1b[38;2;0;15;0m███\x1b[0m\x1b[38;2;0;14;0m███\x1b[0m\x1b[38;2;0;13;0m███\x1b[0m\x1b[38;2;0;12;0m███\x1b[0m\x1b[38;2;0;10;0m███\x1b[0m\x1b[38;2;0;9;0m███\x1b[0m\x1b[38;2;0;8;0m███\x1b[0m\x1b[38;2;0;8;0m███\x1b[0m\x1b[38;2;0;7;0m███\x1b[0m\x1b[38;2;0;6;0m███\x1b[0m\x1b[38;2;0;5;0m███\x1b[0m\x1b[38;2;0;4;0m███\x1b[0m\x1b[38;2;0;2;0m███\x1b[0m\x1b[38;2;0;1;0m███\x1b[0m\x1b[38;2;0;0;0m███\x1b[0m\x1b[38;2;0;0;0m███\x1b[0m\x1b[38;2;15;255;255m███\x1b[0m\x1b[38;2;14;255;255m███\x1b[0m\x1b[38;2;13;255;255m███\x1b[0m\x1b[38;2;12;255;255m███\x1b[0m|
33|\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;254;255;254m███\x1b[0m\x1b[38;2;253;255;253m███\x1b[0m\x1b[38;2;252;255;252m███\x1b[0m|
34|\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;239;255;239m███\x1b[0m\x1b[38;2;238;255;238m███\x1b[0m\x1b[38;2;237;255;237m███\x1b[0m\x1b[38;2;236;255;236m███\x1b[0m|
35|\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;254;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;223;255;223m███\x1b[0m\x1b[38;2;222;255;222m███\x1b[0m\x1b[38;2;221;255;221m███\x1b[0m\x1b[38;2;220;255;220m███\x1b[0m|
36|\x1b[38;2;255;255;207m███\x1b[0m\x1b[38;2;255;255;206m███\x1b[0m\x1b[38;2;255;255;205m███\x1b[0m\x1b[38;2;255;255;204m███\x1b[0m\x1b[38;2;255;255;203m███\x1b[0m\x1b[38;2;255;255;202m███\x1b[0m\x1b[38;2;255;255;201m███\x1b[0m\x1b[38;2;255;255;200m███\x1b[0m\x1b[38;2;255;255;199m███\x1b[0m\x1b[38;2;255;255;198m███\x1b[0m\x1b[38;2;255;255;197m███\x1b[0m\x1b[38;2;254;254;195m███\x1b[0m\x1b[38;2;255;255;195m███\x1b[0m\x1b[38;2;255;255;194m███\x1b[0m\x1b[38;2;255;255;193m███\x1b[0m\x1b[38;2;255;255;192m███\x1b[0m\x1b[38;2;207;255;207m███\x1b[0m\x1b[38;2;206;255;206m███\x1b[0m\x1b[38;2;205;255;205m███\x1b[0m\x1b[38;2;204;255;204m███\x1b[0m|
37|\x1b[38;2;255;255;191m███\x1b[0m\x1b[38;2;255;255;190m███\x1b[0m\x1b[38;2;255;255;189m███\x1b[0m\x1b[38;2;255;255;188m███\x1b[0m\x1b[38;2;255;255;187m███\x1b[0m\x1b[38;2;255;255;186m███\x1b[0m\x1b[38;2;255;255;185m███\x1b[0m\x1b[38;2;255;255;184m███\x1b[0m\x1b[38;2;255;255;183m███\x1b[0m\x1b[38;2;255;255;182m███\x1b[0m\x1b[38;2;254;254;180m███\x1b[0m\x1b[38;2;254;254;179m███\x1b[0m\x1b[38;2;255;255;179m███\x1b[0m\x1b[38;2;255;255;178m███\x1b[0m\x1b[38;2;255;255;177m███\x1b[0m\x1b[38;2;255;255;176m███\x1b[0m\x1b[38;2;191;255;191m███\x1b[0m\x1b[38;2;190;255;190m███\x1b[0m\x1b[38;2;189;255;189m███\x1b[0m\x1b[38;2;188;255;188m███\x1b[0m|
38|\x1b[38;2;255;255;175m███\x1b[0m\x1b[38;2;255;255;174m███\x1b[0m\x1b[38;2;255;255;173m███\x1b[0m\x1b[38;2;255;255;172m███\x1b[0m\x1b[38;2;255;255;171m███\x1b[0m\x1b[38;2;255;255;170m███\x1b[0m\x1b[38;2;255;255;169m███\x1b[0m\x1b[38;2;255;255;168m███\x1b[0m\x1b[38;2;255;255;167m███\x1b[0m\x1b[38;2;255;255;166m███\x1b[0m\x1b[38;2;254;254;164m███\x1b[0m\x1b[38;2;254;254;163m███\x1b[0m\x1b[38;2;255;255;163m███\x1b[0m\x1b[38;2;255;255;162m███\x1b[0m\x1b[38;2;255;255;161m███\x1b[0m\x1b[38;2;255;255;160m███\x1b[0m\x1b[38;2;175;255;175m███\x1b[0m\x1b[38;2;174;255;174m███\x1b[0m\x1b[38;2;173;255;173m███\x1b[0m\x1b[38;2;172;255;172m███\x1b[0m|
39|\x1b[38;2;255;255;159m███\x1b[0m\x1b[38;2;255;255;158m███\x1b[0m\x1b[38;2;255;255;157m███\x1b[0m\x1b[38;2;255;255;156m███\x1b[0m\x1b[38;2;255;255;155m███\x1b[0m\x1b[38;2;255;255;154m███\x1b[0m\x1b[38;2;255;255;153m███\x1b[0m\x1b[38;2;255;255;152m███\x1b[0m\x1b[38;2;255;255;151m███\x1b[0m\x1b[38;2;255;255;150m███\x1b[0m\x1b[38;2;254;254;148m███\x1b[0m\x1b[38;2;254;254;147m███\x1b[0m\x1b[38;2;255;255;147m███\x1b[0m\x1b[38;2;255;255;146m███\x1b[0m\x1b[38;2;255;255;145m███\x1b[0m\x1b[38;2;255;255;144m███\x1b[0m\x1b[38;2;159;255;159m███\x1b[0m\x1b[38;2;158;255;158m███\x1b[0m\x1b[38;2;157;255;157m███\x1b[0m\x1b[38;2;156;255;156m███\x1b[0m|
40|\x1b[38;2;255;255;143m███\x1b[0m\x1b[38;2;255;255;142m███\x1b[0m\x1b[38;2;255;255;141m███\x1b[0m\x1b[38;2;255;255;140m███\x1b[0m\x1b[38;2;255;255;139m███\x1b[0m\x1b[38;2;255;255;138m███\x1b[0m\x1b[38;2;255;255;137m███\x1b[0m\x1b[38;2;255;255;136m███\x1b[0m\x1b[38;2;255;255;135m███\x1b[0m\x1b[38;2;255;255;134m███\x1b[0m\x1b[38;2;254;254;132m███\x1b[0m\x1b[38;2;254;254;131m███\x1b[0m\x1b[38;2;255;255;131m███\x1b[0m\x1b[38;2;255;255;130m███\x1b[0m\x1b[38;2;255;255;129m███\x1b[0m\x1b[38;2;255;255;128m███\x1b[0m\x1b[38;2;143;255;143m███\x1b[0m\x1b[38;2;142;255;142m███\x1b[0m\x1b[38;2;141;255;141m███\x1b[0m\x1b[38;2;140;255;140m███\x1b[0m|
  --------------------------------------------------------------
 > save images/10_out.bmp
Successfully saved image to images/10_out.bmp
 > undo
Switched to layer 2
 > place 3 3 10 s
Switched to layer 4
 > print
    01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20
01|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;254;254m███\x1b[0m\x1b[38;2;255;253;253m███\x1b[0m\x1b[38;2;255;252;252m███\x1b[0m\x1b[38;2;255;251;251m███\x1b[0m\x1b[38;2;255;250;250m███\x1b[0m\x1b[38;2;255;249;249m███\x1b[0m\x1b[38;2;255;248;248m███\x1b[0m\x1b[38;2;255;247;247m███\x1b[0m\x1b[38;2;255;246;246m███\x1b[0m\x1b[38;2;255;245;245m███\x1b[0m\x1b[38;2;255;244;244m███\x1b[0m\x1b[38;2;255;243;243m███\x1b[0m\x1b[38;2;255;242;242m███\x1b[0m\x1b[38;2;255;241;241m███\x1b[0m\x1b[38;2;255;240;240m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;254;255;254m███\x1b[0m\x1b[38;2;253;255;253m███\x1b[0m\x1b[38;2;252;255;252m███\x1b[0m|
02|\x1b[38;2;255;239;239m███\x1b[0m\x1b[38;2;255;238;238m███\x1b[0m\x1b[38;2;255;237;237m███\x1b[0m\x1b[38;2;255;236;236m███\x1b[0m\x1b[38;2;255;235;235m███\x1b[0m\x1b[38;2;255;234;234m███\x1b[0m\x1b[38;2;255;233;233m███\x1b[0m\x1b[38;2;255;232;232m███\x1b[0m\x1b[38;2;255;231;231m███\x1b[0m\x1b[38;2;255;230;230m███\x1b[0m\x1b[38;2;255;229;229m███\x1b[0m\x1b[38;2;255;228;228m███\x1b[0m\x1b[38;2;255;227;227m███\x1b[0m\x1b[38;2;255;226;226m███\x1b[0m\x1b[38;2;255;225;225m███\x1b[0m\x1b[38;2;255;224;224m███\x1b[0m\x1b[38;2;239;255;239m███\x1b[0m\x1b[38;2;238;255;238m███\x1b[0m\x1b[38;2;237;255;237m███\x1b[0m\x1b[38;2;236;255;236m███\x1b[0m|
03|\x1b[38;2;255;223;223m███\x1b[0m\x1b[38;2;255;222;222m███\x1b[0m\x1b[38;2;255;221;221m███\x1b[0m\x1b[38;2;255;220;220m███\x1b[0m\x1b[38;2;255;219;219m███\x1b[0m\x1b[38;2;255;218;218m███\x1b[0m\x1b[38;2;255;217;217m███\x1b[0m\x1b[38;2;255;216;216m███\x1b[0m\x1b[38;2;255;215;215m███\x1b[0m\x1b[38;2;255;214;214m███\x1b[0m\x1b[38;2;255;213;213m███\x1b[0m\x1b[38;2;254;211;211m███\x1b[0m\x1b[38;2;255;211;211m███\x1b[0m\x1b[38;2;255;210;210m███\x1b[0m\x1b[38;2;255;209;209m███\x1b[0m\x1b[38;2;255;208;208m███\x1b[0m\x1b[38;2;223;255;223m███\x1b[0m\x1b[38;2;222;255;222m███\x1b[0m\x1b[38;2;221;255;221m███\x1b[0m\x1b[38;2;220;255;220m███\x1b[0m|
04|\x1b[38;2;255;207;207m███\x1b[0m\x1b[38;2;255;206;206m███\x1b[0m\x1b[38;2;255;205;205m███\x1b[0m\x1b[38;2;255;204;204m███\x1b[0m\x1b[38;2;255;203;203m███\x1b[0m\x1b[38;2;255;202;202m███\x1b[0m\x1b[38;2;255;201;201m███\x1b[0m\x1b[38;2;255;200;200m███\x1b[0m\x1b[38;2;255;199;199m███\x1b[0m\x1b[38;2;255;198;198m███\x1b[0m\x1b[38;2;255;197;197m███\x1b[0m\x1b[38;2;254;195;195m███\x1b[0m\x1b[38;2;255;195;195m███\x1b[0m\x1b[38;2;255;194;194m███\x1b[0m\x1b[38;2;255;193;193m███\x1b[0m\x1b[38;2;255;192;192m███\x1b[0m\x1b[38;2;207;255;207m███\x1b[0m\x1b[38;2;206;255;206m███\x1b[0m\x1b[38;2;205;255;205m███\x1b[0m\x1b[38;2;204;255;204m███\x1b[0m|
05|\x1b[38;2;255;191;191m███\x1b[0m\x1b[38;2;255;190;190m███\x1b[0m\x1b[38;2;255;189;189m███\x1b[0m\x1b[38;2;255;188;188m███\x1b[0m\x1b[38;2;255;187;187m███\x1b[0m\x1b[38;2;255;186;186m███\x1b[0m\x1b[38;2;255;185;185m███\x1b[0m\x1b[38;2;255;184;184m███\x1b[0m\x1b[38;2;255;183;183m███\x1b[0m\x1b[38;2;255;182;182m███\x1b[0m\x1b[38;2;254;180;180m███\x1b[0m\x1b[38;2;254;179;179m███\x1b[0m\x1b[38;2;255;179;179m███\x1b[0m\x1b[38;2;255;178;178m███\x1b[0m\x1b[38;2;255;177;177m███\x1b[0m\x1b[38;2;255;176;176m███\x1b[0m\x1b[38;2;191;255;191m███\x1b[0m\x1b[38;2;190;255;190m███\x1b[0m\x1b[38;2;189;255;189m███\x1b[0m\x1b[38;2;188;255;188m███\x1b[0m|
06|\x1b[38;2;255;175;175m███\x1b[0m\x1b[38;2;255;174;174m███\x1b[0m\x1b[38;2;255;173;173m███\x1b[0m\x1b[38;2;255;172;172m███\x1b[0m\x1b[38;2;255;171;171m███\x1b[0m\x1b[38;2;255;170;170m███\x1b[0m\x1b[38;2;255;169;169m███\x1b[0m\x1b[38;2;255;168;168m███\x1b[0m\x1b[38;2;255;167;167m███\x1b[0m\x1b[38;2;255;166;166m███\x1b[0m\x1b[38;2;254;164;164m███\x1b[0m\x1b[38;2;254;163;163m███\x1b[0m\x1b[38;2;255;163;163m███\x1b[0m\x1b[38;2;255;162;162m███\x1b[0m\x1b[38;2;255;161;161m███\x1b[0m\x1b[38;2;255;160;160m███\x1b[0m\x1b[38;2;175;255;175m███\x1b[0m\x1b[38;2;174;255;174m███\x1b[0m\x1b[38;2;173;255;173m███\x1b[0m\x1b[38;2;172;255;172m███\x1b[0m|
07|\x1b[38;2;255;159;159m███\x1b[0m\x1b[38;2;255;158;158m███\x1b[0m\x1b[38;2;255;157;157m███\x1b[0m\x1b[38;2;255;156;156m███\x1b[0m\x1b[38;2;255;155;155m███\x1b[0m\x1b[38;2;255;154;154m███\x1b[0m\x1b[38;2;255;153;153m███\x1b[0m\x1b[38;2;255;152;152m███\x1b[0m\x1b[38;2;255;151;151m███\x1b[0m\x1b[38;2;255;150;150m███\x1b[0m\x1b[38;2;254;148;148m███\x1b[0m\x1b[38;2;254;147;147m███\x1b[0m\x1b[38;2;255;147;147m███\x1b[0m\x1b[38;2;255;146;146m███\x1b[0m\x1b[38;2;255;145;145m███\x1b[0m\x1b[38;2;255;144;144m███\x1b[0m\x1b[38;2;159;255;159m███\x1b[0m\x1b[38;2;158;255;158m███\x1b[0m\x1b[38;2;157;255;157m███\x1b[0m\x1b[38;2;156;255;156m███\x1b[0m|
08|\x1b[38;2;255;143;143m███\x1b[0m\x1b[38;2;255;142;142m███\x1b[0m\x1b[38;2;255;141;141m███\x1b[0m\x1b[38;2;255;140;140m███\x1b[0m\x1b[38;2;255;139;139m███\x1b[0m\x1b[38;2;255;138;138m███\x1b[0m\x1b[38;2;255;137;137m███\x1b[0m\x1b[38;2;255;136;136m███\x1b[0m\x1b[38;2;255;135;135m███\x1b[0m\x1b[38;2;255;134;134m███\x1b[0m\x1b[38;2;254;132;132m███\x1b[0m\x1b[38;2;254;131;131m███\x1b[0m\x1b[38;2;255;131;131m███\x1b[0m\x1b[38;2;255;130;130m███\x1b[0m\x1b[38;2;255;129;129m███\x1b[0m\x1b[38;2;255;128;128m███\x1b[0m\x1b[38;2;143;255;143m███\x1b[0m\x1b[38;2;142;255;142m███\x1b[0m\x1b[38;2;141;255;141m███\x1b[0m\x1b[38;2;140;255;140m███\x1b[0m|
09|\x1b[38;2;255;127;127m███\x1b[0m\x1b[38;2;255;126;126m███\x1b[0m\x1b[38;2;255;125;125m███\x1b[0m\x1b[38;2;255;124;124m███\x1b[0m\x1b[38;2;255;122;122m███\x1b[0m\x1b[38;2;255;121;121m███\x1b[0m\x1b[38;2;255;121;121m███\x1b[0m\x1b[38;2;255;120;120m███\x1b[0m\x1b[38;2;255;119;119m███\x1b[0m\x1b[38;2;255;118;118m███\x1b[0m\x1b[38;2;255;117;117m███\x1b[0m\x1b[38;2;255;116;116m███\x1b[0m\x1b[38;2;255;114;114m███\x1b[0m\x1b[38;2;255;113;113m███\x1b[0m\x1b[38;2;255;113;113m███\x1b[0m\x1b[38;2;255;112;112m███\x1b[0m\x1b[38;2;127;255;127m███\x1b[0m\x1b[38;2;126;255;126m███\x1b[0m\x1b[38;2;125;255;125m███\x1b[0m\x1b[38;2;124;255;124m███\x1b[0m|
10|\x1b[38;2;255;111;111m███\x1b[0m\x1b[38;2;255;110;110m███\x1b[0m\x1b[38;2;255;146;109m███\x1b[0m\x1b[38;2;0;147;108m███\x1b[0m\x1b[38;2;0;149;106m███\x1b[0m\x1b[38;2;0;150;105m███\x1b[0m\x1b[38;2;0;150;105m███\x1b[0m\x1b[38;2;0;151;104m███\x1b[0m\x1b[38;2;0;152;103m███\x1b[0m\x1b[38;2;0;153;102m███\x1b[0m\x1b[38;2;0;154;101m███\x1b[0m\x1b[38;2;0;155;100m███\x1b[0m\x1b[38;2;0;157;98m███\x1b[0m\x1b[38;2;0;158;97m███\x1b[0m\x1b[38;2;0;158;97m███\x1b[0m\x1b[38;2;0;159;96m███\x1b[0m\x1b[38;2;144;0;111m███\x1b[0m\x1b[38;2;145;0;110m███\x1b[0m\x1b[38;2;146;255;109m███\x1b[0m\x1b[38;2;108;255;108m███\x1b[0m|
11|\x1b[38;2;255;95;95m███\x1b[0m\x1b[38;2;255;94;94m███\x1b[0m\x1b[38;2;255;162;93m███\x1b[0m\x1b[38;2;0;163;92m███\x1b[0m\x1b[38;2;0;165;90m███\x1b[0m\x1b[38;2;0;166;89m███\x1b[0m\x1b[38;2;0;166;89m███\x1b[0m\x1b[38;2;0;167;88m███\x1b[0m\x1b[38;2;0;168;87m███\x1b[0m\x1b[38;2;0;169;86m███\x1b[0m\x1b[38;2;0;170;85m███\x1b[0m\x1b[38;2;0;171;84m███\x1b[0m\x1b[38;2;0;173;82m███\x1b[0m\x1b[38;2;0;174;81m███\x1b[0m\x1b[38;2;0;174;81m███\x1b[0m\x1b[38;2;0;175;80m███\x1b[0m\x1b[38;2;160;0;95m███\x1b[0m\x1b[38;2;161;0;94m███\x1b[0m\x1b[38;2;162;255;93m███\x1b[0m\x1b[38;2;92;255;92m███\x1b[0m|
12|\x1b[38;2;255;79;79m███\x1b[0m\x1b[38;2;255;78;78m███\x1b[0m\x1b[38;2;255;178;77m███\x1b[0m\x1b[38;2;0;179;76m███\x1b[0m\x1b[38;2;0;181;74m███\x1b[0m\x1b[38;2;0;182;73m███\x1b[0m\x1b[38;2;0;182;73m███\x1b[0m\x1b[38;2;0;183;72m███\x1b[0m\x1b[38;2;0;184;71m███\x1b[0m\x1b[38;2;0;185;70m███\x1b[0m\x1b[38;2;0;186;69m███\x1b[0m\x1b[38;2;0;187;68m███\x1b[0m\x1b[38;2;0;189;66m███\x1b[0m\x1b[38;2;0;190;65m███\x1b[0m\x1b[38;2;0;190;65m███\x1b[0m\x1b[38;2;0;191;64m███\x1b[0m\x1b[38;2;176;0;79m███\x1b[0m\x1b[38;2;177;0;78m███\x1b[0m\x1b[38;2;178;255;77m███\x1b[0m\x1b[38;2;76;255;76m███\x1b[0m|
13|\x1b[38;2;255;63;63m███\x1b[0m\x1b[38;2;255;62;62m███\x1b[0m\x1b[38;2;255;194;61m███\x1b[0m\x1b[38;2;0;195;60m███\x1b[0m\x1b[38;2;0;197;58m███\x1b[0m\x1b[38;2;0;198;57m███\x1b[0m\x1b[38;2;0;199;56m███\x1b[0m\x1b[38;2;0;199;56m███\x1b[0m\x1b[38;2;0;200;55m███\x1b[0m\x1b[38;2;0;201;54m███\x1b[0m\x1b[38;2;0;202;53m███\x1b[0m\x1b[38;2;0;203;52m███\x1b[0m\x1b[38;2;0;205;50m███\x1b[0m\x1b[38;2;0;206;49m███\x1b[0m\x1b[38;2;0;207;48m███\x1b[0m\x1b[38;2;0;207;48m███\x1b[0m\x1b[38;2;192;0;63m███\x1b[0m\x1b[38;2;193;0;62m███\x1b[0m\x1b[38;2;194;255;61m███\x1b[0m\x1b[38;2;60;255;60m███\x1b[0m|
14|\x1b[38;2;255;47;47m███\x1b[0m\x1b[38;2;255;46;46m███\x1b[0m\x1b[38;2;255;210;45m███\x1b[0m\x1b[38;2;0;211;44m███\x1b[0m\x1b[38;2;0;213;42m███\x1b[0m\x1b[38;2;0;214;41m███\x1b[0m\x1b[38;2;0;215;40m███\x1b[0m\x1b[38;2;0;215;40m███\x1b[0m\x1b[38;2;0;216;39m███\x1b[0m\x1b[38;2;0;217;38m███\x1b[0m\x1b[38;2;0;218;37m███\x1b[0m\x1b[38;2;0;219;36m███\x1b[0m\x1b[38;2;0;221;34m███\x1b[0m\x1b[38;2;0;222;33m███\x1b[0m\x1b[38;2;0;223;32m███\x1b[0m\x1b[38;2;0;223;32m███\x1b[0m\x1b[38;2;208;0;47m███\x1b[0m\x1b[38;2;209;0;46m███\x1b[0m\x1b[38;2;210;255;45m███\x1b[0m\x1b[38;2;44;255;44m███\x1b[0m|
15|\x1b[38;2;255;31;31m███\x1b[0m\x1b[38;2;255;30;30m███\x1b[0m\x1b[38;2;255;226;29m███\x1b[0m\x1b[38;2;0;227;28m███\x1b[0m\x1b[38;2;0;229;26m███\x1b[0m\x1b[38;2;0;230;25m███\x1b[0m\x1b[38;2;0;231;24m███\x1b[0m\x1b[38;2;0;231;24m███\x1b[0m\x1b[38;2;0;232;23m███\x1b[0m\x1b[38;2;0;233;22m███\x1b[0m\x1b[38;2;0;234;21m███\x1b[0m\x1b[38;2;0;235;20m███\x1b[0m\x1b[38;2;0;237;18m███\x1b[0m\x1b[38;2;0;238;17m███\x1b[0m\x1b[38;2;0;239;16m███\x1b[0m\x1b[38;2;0;239;16m███\x1b[0m\x1b[38;2;224;0;31m███\x1b[0m\x1b[38;2;225;0;30m███\x1b[0m\x1b[38;2;226;255;29m███\x1b[0m\x1b[38;2;28;255;28m███\x1b[0m|
16|\x1b[38;2;255;15;15m███\x1b[0m\x1b[38;2;255;14;14m███\x1b[0m\x1b[38;2;255;242;13m███\x1b[0m\x1b[38;2;0;243;12m███\x1b[0m\x1b[38;2;0;245;10m███\x1b[0m\x1b[38;2;0;246;9m███\x1b[0m\x1b[38;2;0;247;8m███\x1b[0m\x1b[38;2;0;247;8m███\x1b[0m\x1b[38;2;0;248;7m███\x1b[0m\x1b[38;2;0;249;6m███\x1b[0m\x1b[38;2;0;250;5m███\x1b[0m\x1b[38;2;0;251;4m███\x1b[0m\x1b[38;2;0;253;2m███\x1b[0m\x1b[38;2;0;254;1m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;0;255;0m███\x1b[0m\x1b[38;2;240;0;15m███\x1b[0m\x1b[38;2;241;0;14m███\x1b[0m\x1b[38;2;242;255;13m███\x1b[0m\x1b[38;2;12;255;12m███\x1b[0m|
17|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;254;255m███\x1b[0m\x1b[38;2;255;2;255m███\x1b[0m\x1b[38;2;0;3;255m███\x1b[0m\x1b[38;2;0;4;255m███\x1b[0m\x1b[38;2;0;5;255m███\x1b[0m\x1b[38;2;0;6;255m███\x1b[0m\x1b[38;2;0;7;255m███\x1b[0m\x1b[38;2;0;8;255m███\x1b[0m\x1b[38;2;0;9;255m███\x1b[0m\x1b[38;2;0;10;255m███\x1b[0m\x1b[38;2;0;11;255m███\x1b[0m\x1b[38;2;0;12;255m███\x1b[0m\x1b[38;2;0;13;255m███\x1b[0m\x1b[38;2;0;14;255m███\x1b[0m\x1b[38;2;0;15;255m███\x1b[0m\x1b[38;2;0;0;255m███\x1b[0m\x1b[38;2;1;0;255m███\x1b[0m\x1b[38;2;2;255;255m███\x1b[0m\x1b[38;2;252;255;255m███\x1b[0m|
18|\x1b[38;2;255;239;255m███\x1b[0m\x1b[38;2;255;238;255m███\x1b[0m\x1b[38;2;255;18;255m███\x1b[0m\x1b[38;2;0;19;255m███\x1b[0m\x1b[38;2;0;20;255m███\x1b[0m\x1b[38;2;0;21;255m███\x1b[0m\x1b[38;2;0;22;255m███\x1b[0m\x1b[38;2;0;23;255m███\x1b[0m\x1b[38;2;0;24;255m███\x1b[0m\x1b[38;2;0;25;255m███\x1b[0m\x1b[38;2;0;26;255m███\x1b[0m\x1b[38;2;0;27;255m███\x1b[0m\x1b[38;2;0;28;255m███\x1b[0m\x1b[38;2;0;29;255m███\x1b[0m\x1b[38;2;0;30;255m███\x1b[0m\x1b[38;2;0;31;255m███\x1b[0m\x1b[38;2;16;0;255m███\x1b[0m\x1b[38;2;17;0;255m███\x1b[0m\x1b[38;2;18;255;255m███\x1b[0m\x1b[38;2;236;255;255m███\x1b[0m|
19|\x1b[38;2;255;223;255m███\x1b[0m\x1b[38;2;255;222;255m███\x1b[0m\x1b[38;2;255;34;255m███\x1b[0m\x1b[38;2;0;35;255m███\x1b[0m\x1b[38;2;0;36;255m███\x1b[0m\x1b[38;2;0;37;255m███\x1b[0m\x1b[38;2;0;38;255m███\x1b[0m\x1b[38;2;0;39;255m███\x1b[0m\x1b[38;2;0;40;255m███\x1b[0m\x1b[38;2;0;41;255m███\x1b[0m\x1b[38;2;0;42;255m███\x1b[0m\x1b[38;2;1;44;254m███\x1b[0m\x1b[38;2;0;44;255m███\x1b[0m\x1b[38;2;0;45;255m███\x1b[0m\x1b[38;2;0;46;255m███\x1b[0m\x1b[38;2;0;47;255m███\x1b[0m\x1b[38;2;32;0;255m███\x1b[0m\x1b[38;2;33;0;255m███\x1b[0m\x1b[38;2;34;255;255m███\x1b[0m\x1b[38;2;220;255;255m███\x1b[0m|
20|\x1b[38;2;255;207;255m███\x1b[0m\x1b[38;2;255;206;255m███\x1b[0m\x1b[38;2;255;50;255m███\x1b[0m\x1b[38;2;0;51;255m███\x1b[0m\x1b[38;2;0;52;255m███\x1b[0m\x1b[38;2;0;53;255m███\x1b[0m\x1b[38;2;0;54;255m███\x1b[0m\x1b[38;2;0;55;255m███\x1b[0m\x1b[38;2;0;56;255m███\x1b[0m\x1b[38;2;0;57;255m███\x1b[0m\x1b[38;2;0;58;255m███\x1b[0m\x1b[38;2;1;60;254m███\x1b[0m\x1b[38;2;0;60;255m███\x1b[0m\x1b[38;2;0;61;255m███\x1b[0m\x1b[38;2;0;62;255m███\x1b[0m\x1b[38;2;0;63;255m███\x1b[0m\x1b[38;2;48;0;255m███\x1b[0m\x1b[38;2;49;0;255m███\x1b[0m\x1b[38;2;50;255;255m███\x1b[0m\x1b[38;2;204;255;255m███\x1b[0m|
21|\x1b[38;2;255;191;255m███\x1b[0m\x1b[38;2;255;190;255m███\x1b[0m\x1b[38;2;255;66;255m███\x1b[0m\x1b[38;2;0;67;255m███\x1b[0m\x1b[38;2;0;68;255m███\x1b[0m\x1b[38;2;0;69;255m███\x1b[0m\x1b[38;2;0;70;255m███\x1b[0m\x1b[38;2;0;71;255m███\x1b[0m\x1b[38;2;0;72;255m███\x1b[0m\x1b[38;2;0;73;255m███\x1b[0m\x1b[38;2;1;75;254m███\x1b[0m\x1b[38;2;1;76;254m███\x1b[0m\x1b[38;2;0;76;255m███\x1b[0m\x1b[38;2;0;77;255m███\x1b[0m\x1b[38;2;0;78;255m███\x1b[0m\x1b[38;2;0;79;255m███\x1b[0m\x1b[38;2;64;0;255m███\x1b[0m\x1b[38;2;65;0;255m███\x1b[0m\x1b[38;2;66;255;255m███\x1b[0m\x1b[38;2;188;255;255m███\x1b[0m|
22|\x1b[38;2;255;175;255m███\x1b[0m\x1b[38;2;255;174;255m███\x1b[0m\x1b[38;2;255;82;255m███\x1b[0m\x1b[38;2;0;83;255m███\x1b[0m\x1b[38;2;0;84;255m███\x1b[0m\x1b[38;2;0;85;255m███\x1b[0m\x1b[38;2;0;86;255m███\x1b[0m\x1b[38;2;0;87;255m███\x1b[0m\x1b[38;2;0;88;255m███\x1b[0m\x1b[38;2;0;89;255m███\x1b[0m\x1b[38;2;1;91;254m███\x1b[0m\x1b[38;2;1;92;254m███\x1b[0m\x1b[38;2;0;92;255m███\x1b[0m\x1b[38;2;0;93;255m███\x1b[0m\x1b[38;2;0;94;255m███\x1b[0m\x1b[38;2;0;95;255m███\x1b[0m\x1b[38;2;80;0;255m███\x1b[0m\x1b[38;2;81;0;255m███\x1b[0m\x1b[38;2;82;255;255m███\x1b[0m\x1b[38;2;172;255;255m███\x1b[0m|
23|\x1b[38;2;255;159;255m███\x1b[0m\x1b[38;2;255;158;255m███\x1b[0m\x1b[38;2;255;98;255m███\x1b[0m\x1b[38;2;0;99;255m███\x1b[0m\x1b[38;2;0;100;255m███\x1b[0m\x1b[38;2;0;101;255m███\x1b[0m\x1b[38;2;0;102;255m███\x1b[0m\x1b[38;2;0;103;255m███\x1b[0m\x1b[38;2;0;104;255m███\x1b[0m\x1b[38;2;0;105;255m███\x1b[0m\x1b[38;2;1;107;254m███\x1b[0m\x1b[38;2;1;108;254m███\x1b[0m\x1b[38;2;0;108;255m███\x1b[0m\x1b[38;2;0;109;255m███\x1b[0m\x1b[38;2;0;110;255m███\x1b[0m\x1b[38;2;0;111;255m███\x1b[0m\x1b[38;2;96;0;255m███\x1b[0m\x1b[38;2;97;0;255m███\x1b[0m\x1b[38;2;98;255;255m███\x1b[0m\x1b[38;2;156;255;255m███\x1b[0m|
24|\x1b[38;2;255;143;255m███\x1b[0m\x1b[38;2;255;142;255m███\x1b[0m\x1b[38;2;255;114;255m███\x1b[0m\x1b[38;2;0;115;255m███\x1b[0m\x1b[38;2;0;116;255m███\x1b[0m\x1b[38;2;0;117;255m███\x1b[0m\x1b[38;2;0;118;255m███\x1b[0m\x1b[38;2;0;119;255m███\x1b[0m\x1b[38;2;0;120;255m███\x1b[0m\x1b[38;2;0;121;255m███\x1b[0m\x1b[38;2;1;123;254m███\x1b[0m\x1b[38;2;1;124;254m███\x1b[0m\x1b[38;2;0;124;255m███\x1b[0m\x1b[38;2;0;125;255m███\x1b[0m\x1b[38;2;0;126;255m███\x1b[0m\x1b[38;2;0;127;255m███\x1b[0m\x1b[38;2;112;0;255m███\x1b[0m\x1b[38;2;113;0;255m███\x1b[0m\x1b[38;2;114;255;255m███\x1b[0m\x1b[38;2;140;255;255m███\x1b[0m|
25|\x1b[38;2;255;127;255m███\x1b[0m\x1b[38;2;255;126;255m███\x1b[0m\x1b[38;2;255;130;255m███\x1b[0m\x1b[38;2;0;131;255m███\x1b[0m\x1b[38;2;0;133;255m███\x1b[0m\x1b[38;2;0;134;255m███\x1b[0m\x1b[38;2;0;134;255m███\x1b[0m\x1b[38;2;0;135;255m███\x1b[0m\x1b[38;2;0;136;255m███\x1b[0m\x1b[38;2;0;137;255m███\x1b[0m\x1b[38;2;0;138;255m███\x1b[0m\x1b[38;2;0;139;255m███\x1b[0m\x1b[38;2;0;141;255m███\x1b[0m\x1b[38;2;0;142;255m███\x1b[0m\x1b[38;2;0;142;255m███\x1b[0m\x1b[38;2;0;143;255m███\x1b[0m\x1b[38;2;128;0;255m███\x1b[0m\x1b[38;2;129;0;255m███\x1b[0m\x1b[38;2;130;255;255m███\x1b[0m\x1b[38;2;124;255;255m███\x1b[0m|
26|\x1b[38;2;255;111;255m███\x1b[0m\x1b[38;2;255;110;255m███\x1b[0m\x1b[38;2;255;109;255m███\x1b[0m\x1b[38;2;255;108;255m███\x1b[0m\x1b[38;2;255;106;255m███\x1b[0m\x1b[38;2;255;105;255m███\x1b[0m\x1b[38;2;255;105;255m███\x1b[0m\x1b[38;2;255;104;255m███\x1b[0m\x1b[38;2;255;103;255m███\x1b[0m\x1b[38;2;255;102;255m███\x1b[0m\x1b[38;2;255;101;255m███\x1b[0m\x1b[38;2;255;100;255m███\x1b[0m\x1b[38;2;255;98;255m███\x1b[0m\x1b[38;2;255;97;255m███\x1b[0m\x1b[38;2;255;97;255m███\x1b[0m\x1b[38;2;255;96;255m███\x1b[0m\x1b[38;2;111;255;255m███\x1b[0m\x1b[38;2;110;255;255m███\x1b[0m\x1b[38;2;109;255;255m███\x1b[0m\x1b[38;2;108;255;255m███\x1b[0m|
27|\x1b[38;2;255;95;255m███\x1b[0m\x1b[38;2;255;94;255m███\x1b[0m\x1b[38;2;255;93;255m███\x1b[0m\x1b[38;2;255;92;255m███\x1b[0m\x1b[38;2;255;90;255m███\x1b[0m\x1b[38;2;255;89;255m███\x1b[0m\x1b[38;2;255;89;255m███\x1b[0m\x1b[38;2;255;88;255m███\x1b[0m\x1b[38;2;255;87;255m███\x1b[0m\x1b[38;2;255;86;255m███\x1b[0m\x1b[38;2;255;85;255m███\x1b[0m\x1b[38;2;255;84;255m███\x1b[0m\x1b[38;2;255;82;255m███\x1b[0m\x1b[38;2;255;81;255m███\x1b[0m\x1b[38;2;255;81;255m███\x1b[0m\x1b[38;2;255;80;255m███\x1b[0m\x1b[38;2;95;255;255m███\x1b[0m\x1b[38;2;94;255;255m███\x1b[0m\x1b[38;2;93;255;255m███\x1b[0m\x1b[38;2;92;255;255m███\x1b[0m|
28|\x1b[38;2;255;79;255m███\x1b[0m\x1b[38;2;255;78;255m███\x1b[0m\x1b[38;2;255;77;255m███\x1b[0m\x1b[38;2;255;76;255m███\x1b[0m\x1b[38;2;255;74;255m███\x1b[0m\x1b[38;2;255;73;255m███\x1b[0m\x1b[38;2;255;73;255m███\x1b[0m\x1b[38;2;255;72;255m███\x1b[0m\x1b[38;2;255;71;255m███\x1b[0m\x1b[38;2;255;70;255m███\x1b[0m\x1b[38;2;255;69;255m███\x1b[0m\x1b[38;2;255;68;255m███\x1b[0m\x1b[38;2;255;66;255m███\x1b[0m\x1b[38;2;255;65;255m███\x1b[0m\x1b[38;2;255;65;255m███\x1b[0m\x1b[38;2;255;64;255m███\x1b[0m\x1b[38;2;79;255;255m███\x1b[0m\x1b[38;2;78;255;255m███\x1b[0m\x1b[38;2;77;255;255m███\x1b[0m\x1b[38;2;76;255;255m███\x1b[0m|
29|\x1b[38;2;255;63;255m███\x1b[0m\x1b[38;2;255;62;255m███\x1b[0m\x1b[38;2;255;61;255m███\x1b[0m\x1b[38;2;255;60;255m███\x1b[0m\x1b[38;2;255;58;255m███\x1b[0m\x1b[38;2;255;57;255m███\x1b[0m\x1b[38;2;255;56;255m███\x1b[0m\x1b[38;2;255;56;255m███\x1b[0m\x1b[38;2;255;55;255m███\x1b[0m\x1b[38;2;255;54;255m███\x1b[0m\x1b[38;2;255;53;255m███\x1b[0m\x1b[38;2;255;52;255m███\x1b[0m\x1b[38;2;255;50;255m███\x1b[0m\x1b[38;2;255;49;255m███\x1b[0m\x1b[38;2;255;48;255m███\x1b[0m\x1b[38;2;255;48;255m███\x1b[0m\x1b[38;2;63;255;255m███\x1b[0m\x1b[38;2;62;255;255m███\x1b[0m\x1b[38;2;61;255;255m███\x1b[0m\x1b[38;2;60;255;255m███\x1b[0m|
30|\x1b[38;2;255;47;255m███\x1b[0m\x1b[38;2;255;46;255m███\x1b[0m\x1b[38;2;255;45;255m███\x1b[0m\x1b[38;2;255;44;255m███\x1b[0m\x1b[38;2;255;42;255m███\x1b[0m\x1b[38;2;255;41;255m███\x1b[0m\x1b[38;2;255;40;255m███\x1b[0m\x1b[38;2;255;40;255m███\x1b[0m\x1b[38;2;255;39;255m███\x1b[0m\x1b[38;2;255;38;255m███\x1b[0m\x1b[38;2;255;37;255m███\x1b[0m\x1b[38;2;255;36;255m███\x1b[0m\x1b[38;2;255;34;255m███\x1b[0m\x1b[38;2;255;33;255m███\x1b[0m\x1b[38;2;255;32;255m███\x1b[0m\x1b[38;2;255;32;255m███\x1b[0m\x1b[38;2;47;255;255m███\x1b[0m\x1b[38;2;46;255;255m███\x1b[0m\x1b[38;2;45;255;255m███\x1b[0m\x1b[38;2;44;255;255m███\x1b[0m|
31|\x1b[38;2;255;31;255m███\x1b[0m\x1b[38;2;255;30;255m███\x1b[0m\x1b[38;2;255;29;255m███\x1b[0m\x1b[38;2;255;28;255m███\x1b[0m\x1b[38;2;255;26;255m███\x1b[0m\x1b[38;2;255;25;255m███\x1b[0m\x1b[38;2;255;24;255m███\x1b[0m\x1b[38;2;255;24;255m███\x1b[0m\x1b[38;2;255;23;255m███\x1b[0m\x1b[38;2;255;22;255m███\x1b[0m\x1b[38;2;255;21;255m███\x1b[0m\x1b[38;2;255;20;255m███\x1b[0m\x1b[38;2;255;18;255m███\x1b[0m\x1b[38;2;255;17;255m███\x1b[0m\x1b[38;2;255;16;255m███\x1b[0m\x1b[38;2;255;16;255m███\x1b[0m\x1b[38;2;31;255;255m███\x1b[0m\x1b[38;2;30;255;255m███\x1b[0m\x1b[38;2;29;255;255m███\x1b[0m\x1b[38;2;28;255;255m███\x1b[0m|
32|\x1b[38;2;255;15;255m███\x1b[0m\x1b[38;2;255;14;255m███\x1b[0m\x1b[38;2;255;13;255m███\x1b[0m\x1b[38;2;255;12;255m███\x1b[0m\x1b[38;2;255;10;255m███\x1b[0m\x1b[38;2;255;9;255m███\x1b[0m\x1b[38;2;255;8;255m███\x1b[0m\x1b[38;2;255;8;255m███\x1b[0m\x1b[38;2;255;7;255m███\x1b[0m\x1b[38;2;255;6;255m███\x1b[0m\x1b[38;2;255;5;255m███\x1b[0m\x1b[38;2;255;4;255m███\x1b[0m\x1b[38;2;255;2;255m███\x1b[0m\x1b[38;2;255;1;255m███\x1b[0m\x1b[38;2;255;0;255m███\x1b[0m\x1b[38;2;255;0;255m███\x1b[0m\x1b[38;2;15;255;255m███\x1b[0m\x1b[38;2;14;255;255m███\x1b[0m\x1b[38;2;13;255;255m███\x1b[0m\x1b[38;2;12;255;255m███\x1b[0m|
33|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;254m███\x1b[0m\x1b[38;2;255;255;253m███\x1b[0m\x1b[38;2;255;255;252m███\x1b[0m\x1b[38;2;255;255;251m███\x1b[0m\x1b[38;2;255;255;250m███\x1b[0m\x1b[38;2;255;255;249m███\x1b[0m\x1b[38;2;255;255;248m███\x1b[0m\x1b[38;2;255;255;247m███\x1b[0m\x1b[38;2;255;255;246m███\x1b[0m\x1b[38;2;255;255;245m███\x1b[0m\x1b[38;2;255;255;244m███\x1b[0m\x1b[38;2;255;255;243m███\x1b[0m\x1b[38;2;255;255;242m███\x1b[0m\x1b[38;2;255;255;241m███\x1b[0m\x1b[38;2;255;255;240m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;254;255;254m███\x1b[0m\x1b[38;2;253;255;253m███\x1b[0m\x1b[38;2;252;255;252m███\x1b[0m|
34|\x1b[38;2;255;255;239m███\x1b[0m\x1b[38;2;255;255;238m███\x1b[0m\x1b[38;2;255;255;237m███\x1b[0m\x1b[38;2;255;255;236m███\x1b[0m\x1b[38;2;255;255;235m███\x1b[0m\x1b[38;2;255;255;234m███\x1b[0m\x1b[38;2;255;255;233m███\x1b[0m\x1b[38;2;255;255;232m███\x1b[0m\x1b[38;2;255;255;231m███\x1b[0m\x1b[38;2;255;255;230m███\x1b[0m\x1b[38;2;255;255;229m███\x1b[0m\x1b[38;2;255;255;228m███\x1b[0m\x1b[38;2;255;255;227m███\x1b[0m\x1b[38;2;255;255;226m███\x1b[0m\x1b[38;2;255;255;225m███\x1b[0m\x1b[38;2;255;255;224m███\x1b[0m\x1b[38;2;239;255;239m███\x1b[0m\x1b[38;2;238;255;238m███\x1b[0m\x1b[38;2;237;255;237m███\x1b[0m\x1b[38;2;236;255;236m███\x1b[0m|
35|\x1b[38;2;255;255;223m███\x1b[0m\x1b[38;2;255;255;222m███\x1b[0m\x1b[38;2;255;255;221m███\x1b[0m\x1b[38;2;255;255;220m███\x1b[0m\x1b[38;2;255;255;219m███\x1b[0m\x1b[38;2;255;255;218m███\x1b[0m\x1b[38;2;255;255;217m███\x1b[0m\x1b[38;2;255;255;216m███\x1b[0m\x1b[38;2;255;255;215m███\x1b[0m\x1b[38;2;255;255;214m███\x1b[0m\x1b[38;2;255;255;213m███\x1b[0m\x1b[38;2;254;254;211m███\x1b[0m\x1b[38;2;255;255;211m███\x1b[0m\x1b[38;2;255;255;210m███\x1b[0m\x1b[38;2;255;255;209m███\x1b[0m\x1b[38;2;255;255;208m███\x1b[0m\x1b[38;2;223;255;223m███\x1b[0m\x1b[38;2;222;255;222m███\x1b[0m\x1b[38;2;221;255;221m███\x1b[0m\x1b[38;2;220;255;220m███\x1b[0m|
36|\x1b[38;2;255;255;207m███\x1b[0m\x1b[38;2;255;255;206m███\x1b[0m\x1b[38;2;255;255;205m███\x1b[0m\x1b[38;2;255;255;204m███\x1b[0m\x1b[38;2;255;255;203m███\x1b[0m\x1b[38;2;255;255;202m███\x1b[0m\x1b[38;2;255;255;201m███\x1b[0m\x1b[38;2;255;255;200m███\x1b[0m\x1b[38;2;255;255;199m███\x1b[0m\x1b[38;2;255;255;198m███\x1b[0m\x1b[38;2;255;255;197m███\x1b[0m\x1b[38;2;254;254;195m███\x1b[0m\x1b[38;2;255;255;195m███\x1b[0m\x1b[38;2;255;255;194m███\x1b[0m\x1b[38;2;255;255;193m███\x1b[0m\x1b[38;2;255;255;192m███\x1b[0m\x1b[38;2;207;255;207m███\x1b[0m\x1b[38;2;206;255;206m███\x1b[0m\x1b[38;2;205;255;205m███\x1b[0m\x1b[38;2;204;255;204m███\x1b[0m|
37|\x1b[38;2;255;255;191m███\x1b[0m\x1b[38;2;255;255;190m███\x1b[0m\x1b[38;2;255;255;189m███\x1b[0m\x1b[38;2;255;255;188m███\x1b[0m\x1b[38;2;255;255;187m███\x1b[0m\x1b[38;2;255;255;186m███\x1b[0m\x1b[38;2;255;255;185m███\x1b[0m\x1b[38;2;255;255;184m███\x1b[0m\x1b[38;2;255;255;183m███\x1b[0m\x1b[38;2;255;255;182m███\x1b[0m\x1b[38;2;254;254;180m███\x1b[0m\x1b[38;2;254;254;179m███\x1b[0m\x1b[38;2;255;255;179m███\x1b[0m\x1b[38;2;255;255;178m███\x1b[0m\x1b[38;2;255;255;177m███\x1b[0m\x1b[38;2;255;255;176m███\x1b[0m\x1b[38;2;191;255;191m███\x1b[0m\x1b[38;2;190;255;190m███\x1b[0m\x1b[38;2;189;255;189m███\x1b[0m\x1b[38;2;188;255;188m███\x1b[0m|
38|\x1b[38;2;255;255;175m███\x1b[0m\x1b[38;2;255;255;174m███\x1b[0m\x1b[38;2;255;255;173m███\x1b[0m\x1b[38;2;255;255;172m███\x1b[0m\x1b[38;2;255;255;171m███\x1b[0m\x1b[38;2;255;255;170m███\x1b[0m\x1b[38;2;255;255;169m███\x1b[0m\x1b[38;2;255;255;168m███\x1b[0m\x1b[38;2;255;255;167m███\x1b[0m\x1b[38;2;255;255;166m███\x1b[0m\x1b[38;2;254;254;164m███\x1b[0m\x1b[38;2;254;254;163m███\x1b[0m\x1b[38;2;255;255;163m███\x1b[0m\x1b[38;2;255;255;162m███\x1b[0m\x1b[38;2;255;255;161m███\x1b[0m\x1b[38;2;255;255;160m███\x1b[0m\x1b[38;2;175;255;175m███\x1b[0m\x1b[38;2;174;255;174m███\x1b[0m\x1b[38;2;173;255;173m███\x1b[0m\x1b[38;2;172;255;172m███\x1b[0m|
39|\x1b[38;2;255;255;159m███\x1b[0m\x1b[38;2;255;255;158m███\x1b[0m\x1b[38;2;255;255;157m███\x1b[0m\x1b[38;2;255;255;156m███\x1b[0m\x1b[38;2;255;255;155m███\x1b[0m\x1b[38;2;255;255;154m███\x1b[0m\x1b[38;2;255;255;153m███\x1b[0m\x1b[38;2;255;255;152m███\x1b[0m\x1b[38;2;255;255;151m███\x1b[0m\x1b[38;2;255;255;150m███\x1b[0m\x1b[38;2;254;254;148m███\x1b[0m\x1b[38;2;254;254;147m███\x1b[0m\x1b[38;2;255;255;147m███\x1b[0m\x1b[38;2;255;255;146m███\x1b[0m\x1b[38;2;255;255;145m███\x1b[0m\x1b[38;2;255;255;144m███\x1b[0m\x1b[38;2;159;255;159m███\x1b[0m\x1b[38;2;158;255;158m███\x1b[0m\x1b[38;2;157;255;157m███\x1b[0m\x1b[38;2;156;255;156m███\x1b[0m|
40|\x1b[38;2;255;255;143m███\x1b[0m\x1b[38;2;255;255;142m███\x1b[0m\x1b[38;2;255;255;141m███\x1b[0m\x1b[38;2;255;255;140m███\x1b[0m\x1b[38;2;255;255;139m███\x1b[0m\x1b[38;2;255;255;138m███\x1b[0m\x1b[38;2;255;255;137m███\x1b[0m\x1b[38;2;255;255;136m███\x1b[0m\x1b[38;2;255;255;135m███\x1b[0m\x1b[38;2;255;255;134m███\x1b[0m\x1b[38;2;254;254;132m███\x1b[0m\x1b[38;2;254;254;131m███\x1b[0m\x1b[38;2;255;255;131m███\x1b[0m\x1b[38;2;255;255;130m███\x1b[0m\x1b[38;2;255;255;129m███\x1b[0m\x1b[38;2;255;255;128m███\x1b[0m\x1b[38;2;143;255;143m███\x1b[0m\x1b[38;2;142;255;142m███\x1b[0m\x1b[38;2;141;255;141m███\x1b[0m\x1b[38;2;140;255;140m███\x1b[0m|
  --------------------------------------------------------------
 > switch 0
Switched to layer 0
 > print
    01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20
01|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
02|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
03|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
04|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
05|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
06|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
07|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
08|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
09|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
10|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
11|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
12|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
13|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
14|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
15|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
16|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
17|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
18|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
19|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
20|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
21|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
22|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
23|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
24|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
25|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
26|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
27|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
28|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
29|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
30|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
31|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
32|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
33|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
34|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
35|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
36|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
37|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
38|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
39|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
40|\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m\x1b[38;2;255;255;255m███\x1b[0m|
  --------------------------------------------------------------
 > quit
```
</details>