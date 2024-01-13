# StoryTime_Project

StoryTime is a project where you provide your story and the app will turn it into a narrative mode.
You can present your stories to your reader in the way you like it, since the way you want it is always the right way.
and to make this happen, few factors need to be set by you, like narration speed, pauses, background and tone.
this is the project idea i'm going to work on.

The project has just started and still ongoing and i will try to upload new updates frequently, but for now this is what the project does.
the project has 2 different builds, one for the writer and the other for the reader.
+ Writer release:
 - Since the whole idea is about making a story, I thought it's better to prevent the reader from revealing its events and preserve the way the writer want to present the story.So i just included an encryption system to the project to encrypt the story.
+ Reader release:
 - Decrypt the story file and run the narration mode

How it works:
+ Writer:
 - Before you build the project:
    Provide your story as a text.txt file, and in the config section, set the file name
    Set the encrypted file name you will generate
    Make sure to define "BUILD_FOR_APPLICANT", the project already includes it in config section, just uncomment it
 - In Runtime, the app will automatically: 
    Generate Encryption(public)/Decryption(private) keys, by the way, the encryption system used is RSA
    Encrypt the file and generate the encrypted file
 - After running the App and before you close it make sure to save the keys and send the private key to the reader. i'm using those terms 'private key', 'public key' but in fact each one is a pair of keys:
   Private key includes: decryption key and modulus value
   Public key includes: encryption key and modulus value
   sice the Prived key include the decryption key, you need to send that to the reader or build the app with that key and send the app to the reader
+ Reader:
 - before you build the app:
   make sure to remove or comment the define 'BUILD_FOR_APPLICANT'
   you have the option to pre-set the private key in config section so the reader won't need to set them up
 -App runtime and one of the 3 cases will happen
   If the reader runs the app using cmd line and provides the private key (like this: App.exe <decryption key> <modulus value>), the app will consider those as private key and use them for decryption
   If the App runs without parameters (using App.exe directly), the app will check if Private Key pre-set, and use them for decryption
   If no private key was pre-set, it will ask the reader for those keys
 -The app will decrypt the file and start


Text format:
The application provide some options for writer to control the story narration:
 + '$': any tag or narration option should be surrounded by this symbol '$'
 + $start_rythm$ => rythm is the narration speed, and this command will start or enable the narration rythm, it's enabled by default
 + $stop_rythm$ => disable narration rythm 
 + $pause_s=$ => pause narration for a duration in seconds
 + $pause_ms=$ => pause narration for a duration in milli seconds
 + $skip_on_yes=$ => this will pop up a yes or no question for the reader, asking him if he want's to skip text by entering 'y' until the $skip_end$ tag detected, you write your question after '=' 
 + $skip_on_no=$ => same as $skip_on_yes=$ but it will skip if 'n' is entered
 + $skip_end$ => the end of skip tag
 Text style:
 + $text_font=italic$ => italic font
 + $text_font=bold$ => bold font
 + $text_font=underline$ => underline text
 + $text_font=default$ => user default text font
 + $text_color=white$ => white text
 + $text_color=black$ => black text
 + $text_color=red$ => red text
 + $text_color=blue$ => blue text
 + $text_color=green$ => green text
 + $text_color=yellow$ => yellow text
 + $text_color=cyna$ => cyna text
 + $text_color=magenta$ => magenta text
 + $text_color=bright_white$ => bright white text
 + $text_color=bright_black$ => bright black text
 + $text_color=bright_red$ => bright red text
 + $text_color=bright_blue$ => bright blue text
 + $text_color=bright_green$ => bright green text
 + $text_color=bright_yellow$ => bright yellow text
 + $text_color=bright_cyna$ => bright cyna text
 + $text_color=bright_magenta$ => bright magenta text
 + $text_color=default$ => use default texe color
 + $text_style_default$ => clear all text attributs (color + font)