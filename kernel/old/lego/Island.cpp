BOOL IsleApp::ReadReg(LPCSTR name, LPSTR outValue, DWORD outSize)
{
  HKEY hKey;
  DWORD valueType;

  BOOL out = FALSE;
  DWORD size = outSize;
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Mindscape\\LEGO Island", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
    if (RegQueryValueExA(hKey, name, NULL, &valueType, (LPBYTE) outValue, &size) == ERROR_SUCCESS) {
      if (RegCloseKey(hKey) == ERROR_SUCCESS) {
        out = TRUE;
      }
    }
  }

  return out;
}

// OFFSET: ISLE 0x4027b0
int IsleApp::ReadRegBool(LPCSTR name, BOOL *out)
{
  char buffer[256];

  BOOL read = ReadReg(name, buffer, sizeof(buffer));
  if (read) {
    if (strcmp("YES", buffer) == 0) {
      *out = TRUE;
      return read;
    }

    if (strcmp("NO", buffer) == 0) {
      *out = FALSE;
      return read;
    }

    read = FALSE;
  }
  return read;
}

// OFFSET: ISLE 0x402880
int IsleApp::ReadRegInt(LPCSTR name, int *out)
{
  char buffer[256];

  BOOL read = ReadReg(name, buffer, sizeof(buffer));
  if (read) {
    *out = atoi(buffer);
  }

  return read;
}

// OFFSET: ISLE 0x4028d0
void IsleApp::LoadConfig()
{
  char buffer[1024];

  if (!ReadReg("diskpath", buffer, sizeof(buffer))) {
    strcpy(buffer, MxOmni::GetHD());
  }

  m_hdPath = new char[strlen(buffer) + 1];
  strcpy(m_hdPath, buffer);
  MxOmni::SetHD(m_hdPath);

  if (!ReadReg("cdpath", buffer, sizeof(buffer))) {
    strcpy(buffer, MxOmni::GetCD());
  }

  m_cdPath = new char[strlen(buffer) + 1];
  strcpy(m_cdPath, buffer);
  MxOmni::SetCD(m_cdPath);

  ReadRegBool("Flip Surfaces", &m_flipSurfaces);
  ReadRegBool("Full Screen", &m_fullScreen);
  ReadRegBool("Wide View Angle", &m_wideViewAngle);
  ReadRegBool("3DSound", &m_use3dSound);
  ReadRegBool("Music", &m_useMusic);
  ReadRegBool("UseJoystick", &m_useJoystick);
  ReadRegInt("JoystickIndex", &m_joystickIndex);
  ReadRegBool("Draw Cursor", &m_drawCursor);

  int backBuffersInVRAM;
  if (ReadRegBool("Back Buffers in Video RAM",&backBuffersInVRAM)) {
    m_backBuffersInVram = !backBuffersInVRAM;
  }

  int bitDepth;
  if (ReadRegInt("Display Bit Depth", &bitDepth)) {
    if (bitDepth == 8) {
      m_using8bit = TRUE;
    } else if (bitDepth == 16) {
      m_using16bit = TRUE;
    }
  }

  if (!ReadReg("Island Quality", buffer, sizeof(buffer))) {
    strcpy(buffer, "1");
  }
  m_islandQuality = atoi(buffer);

  if (!ReadReg("Island Texture", buffer, sizeof(buffer))) {
    strcpy(buffer, "1");
  }
  m_islandTexture = atoi(buffer);

  if (ReadReg("3D Device ID", buffer, sizeof(buffer))) {
    m_deviceId = new char[strlen(buffer) + 1];
    strcpy(m_deviceId, buffer);
  }

  if (ReadReg("savepath", buffer, sizeof(buffer))) {
    m_savePath = new char[strlen(buffer) + 1];
    strcpy(m_savePath, buffer);
  }
}
//error gen
