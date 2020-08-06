using System;
using System.IO;
using System.Runtime.InteropServices;
using UnityEngine;

delegate int pow2(int a, string jreHome);

public class J4CsScript : MonoBehaviour
{
    private IntPtr dll;

    // Start is called before the first frame update
    void Start()
    {
        var binaryOutputDirectory = Path.GetFullPath("../cmake-build-release-vs");
        dll = Native.LoadLibrary(binaryOutputDirectory + "/j4cs.dll");
        if (dll == IntPtr.Zero)
        {
            Debug.LogError("Failed to load native library");
        }
        else
        {
            Debug.Log("Native library loaded");
        
            var result = Native.Invoke<int, pow2>(dll, 12, binaryOutputDirectory + "/jre");
            Debug.Log("Pow2: " + result);
        }
    }

    private void OnDestroy()
    {
        Native.FreeLibrary(dll);
        Debug.Log("native library unloaded");
    }

    // Update is called once per frame
    void Update()
    {
    }
}