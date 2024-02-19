//PS2
s32 JonMaskFPS(s32 val,s32 add)
{
    return add * 0x3c + val <= 0x3c0000 ? add * 0x3c + val : (add * 0x3c + val) - 0x3c0000;
}