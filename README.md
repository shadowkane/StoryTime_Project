# StoryTime_Project
![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/shadowkane/StoryTime_Project/workflow.yml)
![GitHub License](https://img.shields.io/github/license/shadowkane/StoryTime_Project)
![GitHub top language](https://img.shields.io/github/languages/top/shadowkane/StoryTime_Project)
![Windows](https://img.shields.io/badge/Windows-0078D6?logo=windows&logoColor=white)
![Ubuntu](https://img.shields.io/badge/Ubuntu-E95420?logo=ubuntu&logoColor=white)

StoryTime is a project where you provide your story and the app will turn it into a narrative mode.
You can present your stories to your reader in the way you like it, because the way you want it is always the right way.
and to make this happen, few factors need to be set by you, like narration speed, text color and font, background sound and vocal tone.
this is the project idea i'm working on.

The project has just started and still ongoing and i will try to update it frequently, but for now this is what the project does.
the project has 2 different builds, one for the writer and the other for the reader.
### Writer version:
  + Since the whole idea is about making a story, I believe, it's better to protect the reader from spoilers and prevent him from having access to the raw story text to preserve the way the writer wants to present his story. To do so, we have to :
    + Light weight application.
    + One application for all stories.
    + Easy to use by the writer.
    + Protects the raw story text from the readers
  + Solution is :
    + Use an Encryption system to protection the story.
    + Use directives or tags to set the narration style.
  + Show a preview of the story.
### Reader version:
  + Decrypt the story file and run the narration module.

# Build
To build the project, you can use the make command with a variety of options:
>`make TARGET=<target_name> <build_type> [debug_level=<debug_level_number>]`
- <Target_name>: writer, reader or test. (required)
- <build_type>: release or debug. release is by default. only for TARGET=writer or reader. (opional)
- <debug_level_number>: a number between 0..3, 0 mean show no information. by default is 0 and works only with `debug`. (optional)
> `make clean [TARGET=<target_name>]`
- <target_name>: specify the build target you want to clean, writer, reader or test. if you don't specify the target, the command will clean the entire build folder for reader and writer.
### examples:
`make TARGET=test` => test the project modules.

`make TARGET=writer` => build a release version for the writer.

`make TARGET=reader debug debug_level=2` => build a debug version for the reader with debug level = 2.

`make clean TARET=test` => clean test build folder

`make clean` => clean the entire build folder

# How it works
You need to build two versions, writer and reader, this is how each one is working: 
### Writer:
- Preparation and Configuration:
  + Provide your story as a text file and use tags to customize your narration style.
  + In config file, set the path to your story and the path where to save the encrypted story, also you can change the application title (for example, change it to the story name).
- Application runtime:
  + In case the story file path was incorrect, the application will ask you to set a valid path to your story file.
  + Next, the application will automatically generate the encryption and decryption key pair along side with the encryption version of your story.
  + You will have the option to preview the results without the need of the reader version.

  > [!IMPORTANT]
  > Make sure to save the decryption key pair to use it in the reader version of the application.

  > [!NOTE] 
  >By the way, the encryption system used is RSA
  
### Reader:
- Preparation and Configuration:
  + One of the options to set the decryption key pair is to save it in the config.h file before building the reader version of the project.
  + In case you need to change the path to the encrypted story, you can edit the config.h file to meet your expectations.
- Application runtime:
  + The Application provides more than one option to set the encryption key pair, beside the config.h file, the application will prioritize the key pair that passed as arguments.(like this: `StoryTime <decryption_key> <modulus_value>`)
  + If the App runs without arguments (using just `StoryTime.exe`), it will check for the default decryption key pair, (those in the config.h file)
  + If none of the previous option was verified, the App will ask the user (reader) to set the decryption key pair.
  + The app will check the existence of the story file, if it's not exist, the reader will be required to enter the path to the encrypted story.
  + After the decryption key pair and the story file path being set correctly, the application will start the narration of the story.

# Narration Style
The application provide some options for writer to control the story narration:
 + Any tag or narration option should be surrounded by this symbol '$'. (exp: \$\<tag>\$)
 + Tags:
    + start_rythm => rythm is the narration speed, and this command will start or enable the narration rythm, it's enabled by default
    + stop_rythm => disable narration rythm
    + rythm_default => set narration rythm speed to default
    + rythm_s=<numbfer_of_seconds> => set narration rythm speed to 1 char per <numbfer_of_seconds> second
    + rythm_ms=<numbfer_of_milliseconds> => set narration rythm speed to 1 char per <numbfer_of_milliseconds> milliseconds
    + pause_s=<pause_duration> => pause narration for a duration in seconds
    + pause_ms=<pause_duration> => pause narration for a duration in milli seconds
    + skip_on_yes=what you want to say => this will pop up a yes or no question for the reader, asking him if he want's to skip text by entering 'y' until the \$skip_end\$ tag detected, you write your question after '=' or leae it emty
    + skip_on_no=what you want to say => same as $skip_on_yes=$ but it will skip if 'n' is entered
    + skip_end => the end of skip tag
    + Text style:
      + Font:
        + text_font=italic => italic font
        + text_font=bold => bold font
        + text_font=underline => underline text
        + text_font=default => user default text font
      + Color:
        + text_color=white => white text
        + text_color=black => black text
        + text_color=red => red text
        + text_color=blue => blue text
        + text_color=green => green text
        + text_color=yellow => yellow text
        + text_color=cyna => cyna text
        + text_color=magenta => magenta text
        + text_color=bright_white => bright white text
        + text_color=bright_black => bright black text
        + text_color=bright_red => bright red text
        + text_color=bright_blue => bright blue text
        + text_color=bright_green => bright green text
        + text_color=bright_yellow => bright yellow text
        + text_color=bright_cyna => bright cyna text
        + text_color=bright_magenta => bright magenta text
        + text_color=default => use default texe color
      + text_style_default => clear all text attributs (color + font)

### [This is a link to an example of a text](docs/aboutMe.txt), you can see results in the next section.
# Results
A writer version in windows
![alt text](<docs/images/StoryTime windows writer.JPG>)

A reader version in ubuntu
![alt text](<docs/images/StoryTime ubuntu reader.JPG>)
