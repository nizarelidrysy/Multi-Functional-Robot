import os
import re
import tkinter as tk
from tkinter import filedialog
reset = "\033[0m\033[40m"
bold = "\033[1m"
underline = "\033[4m"
gray = "\033[1;30m"
black = "\033[30m"
red = "\033[31m"
green = "\033[32m"
yellow = "\033[33m"
cyan = "\033[36m"
white = "\033[37m"
hblue = "\033[44m"
hbluegreenbold = "\033[44;38;5;82;1m"
hcyan = "\033[46m"
hgray = "\033[100m"
hred = "\033[41m"
hgreen = "\033[42m"
hyellow = "\033[43m"
hblack = "\033[40m"
def setup_automation():
    root = tk.Tk()
    root.withdraw()
    print("\nWelcome to the "+bold+underline+"DAMI 3000"+reset+cyan+".ino"+reset+bold+" mode adder wizard !"+reset)
    print("\nPlease select your "+bold+underline+"DAMI 3000"+reset+" "+cyan+".ino"+reset+" file...")
    ino_file_path = filedialog.askopenfilename(
        title="Select "+bold+underline+"DAMI 3000"+reset+" "+cyan+".ino"+reset+" File",
        filetypes=[("Arduino Files", "*.ino"), ("All Files", "*.*")],
    )
    if not ino_file_path:
        print(red+"No file selected. Exiting..."+reset)
        return
    file_name = os.path.basename(ino_file_path)
    print(f"\n"+green+"Found project file: "+reset+cyan+underline+file_name+reset+cyan+"."+reset+"\n")
    mode_input = input(yellow+bold+"Enter mode name:"+reset+bold+" \n\n > ").strip()
    mode_raw = re.sub(r"\s+", "_", mode_input).upper()
    display_name = mode_input.upper()
    mode_key = input("\n"+yellow+"Enter keypad trigger key:"+reset+"\n"+yellow+underline+"Layout:"+reset+bold+"\n\n'1', '2', '3', 'A'\n'4', '5', '6', 'B'\n'7', '8', '9', 'C'\n'*', '0', '#', 'D'\n\n > "+reset).strip()
    wants_pause = (
        input("\n"+yellow+bold+"Do you want this mode to support pausing ? "+reset+bold+"(y/n)"+reset+bold+"\n\n > ")
        .strip()
        .lower()
        == "y"
    )
    mode_title = " ".join([w.capitalize() for w in mode_raw.split("_")])
    comment_header = f"// {mode_title} mode"
    func_name = f"handle{mode_title.replace(' ', '')}"
    with open(ino_file_path, "r", encoding="utf-8") as f:
        content = f.read()
    enum_match = re.search(r"(enum\s+Mode\s*\{([^}]+)\})", content)
    if enum_match:
        full_enum_block = enum_match.group(1)
        enum_inner = enum_match.group(2)
        if mode_raw not in enum_inner:
            new_enum_inner = re.sub(
                r"(\s*)(\w+)(\s*)$", rf"\1\2,\n  {mode_raw}\3", enum_inner
            )
            content = content.replace(
                full_enum_block,
                full_enum_block.replace(enum_inner, new_enum_inner),
            )
            print(f"\n"+hgreen+bold+"[✓] Added {mode_raw} to enum Mode."+reset+"")
    dec_pattern = r"(// Manual Test mode\s+void handleManualTest)"
    if func_name not in content and re.search(dec_pattern, content):
        content = re.sub(
            dec_pattern,
            f"{comment_header}\nvoid {func_name}();\n\\1",
            content,
            count=1,
        )
        print(
            f""+hgreen+bold+"[✓] Inserted forward declaration header tightly above Manual Test."+reset+""
        )
    trigger_block = f"""    else if ((customKey == '{mode_key}' || (Serial1.available() && Serial1.peek() == '{mode_key}')) && currentMode == NONE)
    {{
      if (Serial1.available() && Serial1.peek() == '{mode_key}')
      {{
        Serial1.read();
      }}
      buzzer_activeBeep();
      lcd.clear();
      digitalWrite(LED_red, LOW);
      digitalWrite(LED_green, LOW);
      digitalWrite(LED_blue, LOW);
      sevenSegment_clearAllSegments();
      currentMode = {mode_raw};
    }}"""
    loop_marker = "  else if (currentMode == BLUETOOTH)"
    if trigger_block not in content and loop_marker in content:
        content = content.replace(
            loop_marker, f"{trigger_block}\n  else if (currentMode == BLUETOOTH)"
        )
        print(f""+hgreen+bold+"[✓] Added keypad/serial initialization trigger."+reset+"")
    if wants_pause:
        execution_hook = f"""  else if (currentMode == {mode_raw})
  {{
    if (!isMazePaused)
    {{
      {func_name}();
    }}
  }}"""
    else:
        execution_hook = f"""  else if (currentMode == {mode_raw})
  {{
    {func_name}();
  }}"""
    follow_me_pattern = "  else if (currentMode == FOLLOW_ME)\n  {\n    if (!isMazePaused)\n    {\n      handleFollowMe();\n    }\n  }"
    if execution_hook not in content and follow_me_pattern in content:
        content = content.replace(
            follow_me_pattern, f"{follow_me_pattern}\n{execution_hook}"
        )
        print(f""+hgreen+"[✓] Added continuous execution hook inside loop()."+reset+"")
    if wants_pause:
        content = content.replace(
            "if (currentMode == MAZE || currentMode == BLUETOOTH || currentMode == FOLLOW_ME)",
            f"if (currentMode == MAZE || currentMode == BLUETOOTH || currentMode == FOLLOW_ME || currentMode == {mode_raw})",
        )
        content = content.replace(
            "if ((currentMode == MAZE || currentMode == BLUETOOTH || currentMode == FOLLOW_ME) && isMazePaused)",
            f"if ((currentMode == MAZE || currentMode == BLUETOOTH || currentMode == FOLLOW_ME || currentMode == {mode_raw}) && isMazePaused)",
        )
        print(""+hgreen+bold+"[✓] Linked mode directly to system-wide pause logic."+reset+"")
    func_implementation = (
        f"{comment_header}\n"
        f"void {func_name}()\n"
        f"{{\n"
        f"  // Insert code here\n"
        f"}}\n"
    )
    impl_pattern = r"(// Manual Test mode\s+)?void handleManualTest\([^)]*\)\s*\{"
    match = re.search(impl_pattern, content)
    if match:
        start_idx = match.start()
        content = content[:start_idx] + func_implementation + content[start_idx:]
        print(
            f""+hgreen+bold+"[✓] Placed functional code block tightly above Manual Test implementation."+reset+""
        )
    else:
        content += f"\n{func_implementation}"
        print(f""+hgreen+"[✓] Appended functional code block to the end of the file."+reset+"")
    with open(ino_file_path, "w", encoding="utf-8") as f:
        f.write(content)
    print(f"\n"+green+bold+"Successfully updated "+reset+cyan+underline+file_name+reset+cyan+"."+reset+"\n")
if __name__ == "__main__":
    setup_automation()