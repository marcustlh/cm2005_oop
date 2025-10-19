# cm2005_oop - Otodecks DJ Application

## Overview
Otodecks is a dual-deck DJ mixing application developed using the JUCE C++ framework. It enables users to load, mix, and manipulate two music tracks simultaneously with intuitive controls and a drag-and-drop playlist system. The application provides waveform visualization, reverb effects, looping, playback speed, and volume control, designed for a seamless and interactive DJing experience.

## Key Features
- Dual-deck playback with independent controls for left and right channels  
- Drag-and-drop playlist functionality for quick file loading  
- Searchable music library with duration display  
- Waveform display with playhead tracking  
- Real-time volume, speed, and position sliders  
- Reverb effects with adjustable balance, damping, and room size parameters  
- Loop mode for continuous playback  
- “Up Next” queue display for each deck  

## Project Structure
| File | Description |
|------|--------------|
| **Main.cpp / MainComponent.cpp** | Application entry point and main UI layout |
| **DJAudioPlayer.cpp** | Handles audio playback, transport control, and reverb effects |
| **DeckGUI.cpp** | Implements user interface for each deck with waveform, sliders, and playback controls |
| **PlaylistComponent.cpp** | Manages the music library, file drag-and-drop, and search functionality |
| **WaveformDisplay.cpp** | Visualizes track waveforms and current playback position |

## Technologies Used
- **JUCE Framework** for GUI and audio processing  
- **C++17** for application logic  
- **ReverbAudioSource** for built-in reverb effects  
- **AudioTransportSource** for playback and audio control  
- **AudioThumbnail** for waveform visualization  

## Core Functionalities
### 1. Audio Playback and Mixing
- Each deck uses an independent `DJAudioPlayer` connected to the mixer source.  
- Users can load, play, pause, and loop individual tracks.  
- Volume and playback speed are adjustable in real-time.

### 2. Playlist Management
- Tracks can be added via drag-and-drop.  
- The playlist includes title, duration, and buttons to load tracks into either deck.  
- A text search bar filters tracks dynamically.

### 3. Waveform Visualization
- Real-time waveform rendered using `AudioThumbnail`.  
- Displays playback progress and current track name.  
- Updates continuously using a JUCE timer callback.

### 4. Reverb and Audio Effects
Implemented via the JUCE `Reverb` class with adjustable parameters:
- **Balance (Dry/Wet):** Controls mix ratio between unprocessed and reverberated sound.  
- **Damping:** Adjusts high-frequency absorption within the virtual room.  
- **Room Size:** Defines the reverberation environment size.

### 5. User Interface Layout
- Two symmetrical deck GUIs positioned side by side.  
- Playlist component at the bottom for drag-and-drop and search.  
- Labels categorize waveform, widget controls, and playlist sections for clarity.

## How to Build and Run
1. Open the project in the **Projucer** (JUCE IDE).  
2. Configure exporter settings for your platform (e.g., Visual Studio, Xcode).  
3. Save and open in your IDE.  
4. Build and run the application.  
5. Drag audio files into the playlist to begin mixing.

## Future Improvements
- Add crossfader for smoother deck transitions  
- Integrate EQ controls and filter effects  
- Enhance visual feedback with level meters  
- Implement file persistence for playlist saving and loading  
- Add dark/light theme toggle

## Author
**Marcus Tan Lai He**  

## License
This project is released under the MIT License.
