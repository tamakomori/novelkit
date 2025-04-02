// This function is called when the app wants to open a window.
func setup() {
    // Return a window configuration.
    return {
        title: "Sample",
        width: 640,
        height: 480
    };
}

// This function is called when the app wants to start a game.
func first() {
    // Move to a scenario file.
    NovelKit.moveToScenario({file: "title.txt"});
}

//
// The functions below can be called from scenario files.
//
