nativeClass = new NativeClass();
nativeClass.voidMethodInt(12);
count = nativeClass.Count;
str = nativeClass.stringMethodStringInt("xyz_", count + 1);
strs = splitString(str, "_");
strs = splitString(strs[1], "y");