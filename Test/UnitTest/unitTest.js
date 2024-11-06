strs = splitString(":bcd", ":");
strs = splitString("::bcd", ":");
strs = splitString("a:bcd", ":");
strs = splitString("a::cd", ":");
strs = splitString("abc::", ":");
strs = splitString("abcd:", ":");
strs = splitString("abcde", ":");
strs = splitString("a:c:d", ":");
strs = splitString(":bcd", "::");
strs = splitString("::bcd", "::");
strs = splitString("a::cd", "::");
strs = splitString("abc::", "::");
strs = splitString("abcde", "::");

nativeClass = new NativeClass();
nativeClass.voidMethodInt(12);
count = nativeClass.Count;
str = nativeClass.stringMethodStringInt("xyz_", count + 1);
