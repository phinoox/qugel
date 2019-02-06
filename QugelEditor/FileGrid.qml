import QtQuick 2.5
import Qt.labs.folderlistmodel 2.2
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4


Rectangle {
    id: fileBrowser
    objectName: "fileBrowser"
    color: "transparent"
    z: 4

    property string folder
    onFolderChanged: if(loader.item) loader.item.folders.folder="file://"+folder;
    property bool shown: false
    onShownChanged: show()
    signal fileSelected(string file)
    signal fileActivated(string file)
    signal pathChanged(string msg)

    function selectFile(file) {
        if (file !== "") {
            folder = loader.item.folders.folder
            fileBrowser.fileSelected(file)
        }

    }



    Loader {
        id: loader
    }

    function show() {
        loader.sourceComponent = fileBrowserComponent
        loader.item.parent = fileBrowser
        loader.item.anchors.fill = fileBrowser
        loader.item.folder = fileBrowser.folder
    }

    Component {
        id: fileBrowserComponent

        Rectangle {
            id: root
            color: "black"
            property bool showFocusHighlight: false
            property variant folders: folders1
            property variant view: view1
            property alias folder: folders1.folder
            property color textColor: "white"
            property int itemHeight : 64
            property int imageWidth: 64
            property int itemWidth : 96
            property int fontSize : 12
            property int scaledMargin : 2
            anchors.fill: parent


            FolderListModel {
                id: folders1
                folder: folder
            }

            FolderListModel {
                id: folders2
                folder: folder
            }

            SystemPalette {
                id: palette
            }

            Component {
                id: folderDelegate

                Button {
                    id: wrapper

                    function launch() {
                        var path = "file://";
                        if (filePath.length > 2 && filePath[1] === ':') // Windows drive logic, see QUrl::fromLocalFile()
                            path += '/';
                        path += filePath;
                        if (folders.isFolder(index)){
                            goDown(path);
                            fileBrowser.pathChanged(path)
                        }
                        else
                            fileBrowser.fileSelected(path)

                    }

                    function activateItem(){
                        var path = "file://";
                        if (filePath.length > 2 && filePath[1] === ':') // Windows drive logic, see QUrl::fromLocalFile()
                            path += '/';
                        path += filePath;
                        if (folders.isFolder(index)){

                        }
                        else
                            fileBrowser.fileActivated(path)
                    }
                    width: itemWidth
                    height: itemHeight+fontSize+4

                    ToolTip.visible: hovered
                    ToolTip.delay: 750
                    ToolTip.timeout: 5000
                    ToolTip.text: fileName
                    background: Rectangle {
                                color: "transparent"
                                border.color: "#26282a"
                                border.width: 0
                                radius: 4
                            }


                    Rectangle {
                        id: highlight; visible: false
                        anchors.fill: parent
                        color: palette.highlight
                        gradient: Gradient {
                            GradientStop { id: t1; position: 0.0; color: palette.highlight }
                            GradientStop { id: t2; position: 1.0; color: Qt.lighter(palette.highlight) }
                        }
                    }

                    Item {
                        width: imageWidth; height: itemHeight
                        Image {
                            source: folders.isFolder(index)?"qrc:/images/icon_Folder.svg" :"qrc:/images/text-plain.svg"
                            fillMode: Image.PreserveAspectFit

                          anchors.horizontalCenter:parent.horizontalCenter

                        }



                    }

                    Label {
                        id: nameText
                        width: itemWidth
                        anchors.fill: parent; verticalAlignment: Text.AlignBottom;horizontalAlignment: Text.AlignLeft
                        text: fileName
                        elide: Text.ElideRight
                        anchors.topMargin: 16
                        font.pointSize:  fontSize
                        color: (wrapper.ListView.isCurrentItem && root.showFocusHighlight) ? palette.highlightedText : textColor
                        MouseArea {

                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton | Qt.RightButton

                            onDoubleClicked:{console.log("Double Click");}


                        }


                    }


                    MouseArea {
                        id: mouseRegion
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton | Qt.RightButton

                        onDoubleClicked:{console.log("Double Click");}

//                        onClicked: { if (folders == wrapper.GridView.view.model) launch() }
                        drag.target:wrapper
                       // onDoubleClicked: activateItem();
                    }

                    states: [
                        State {
                            name: "pressed"
                            when: mouseRegion.pressed
                            PropertyChanges { target: highlight; visible: true }
                            PropertyChanges { target: nameText; color: palette.highlightedText }
                        }
                    ]
                }
            }

            GridView {
                id: view1
                anchors.top: parent.top;
                anchors.bottom:parent.bottom;

                x: 0
                width: parent.width
                model: folders1
                delegate: folderDelegate
                highlight: Rectangle {
                    color: palette.highlight
                    visible: root.showFocusHighlight && view1.count != 0
                    gradient: Gradient {
                        GradientStop { id: t1; position: 0.0; color: palette.highlight }
                        GradientStop { id: t2; position: 1.0; color: Qt.lighter(palette.highlight) }
                    }
                    width: view1.currentItem == null ? 0 : view1.currentItem.width
                }
                //highlightMoveVelocity: 1000
                pressDelay: 100
                focus: true
                state: "current"
                states: [
                    State {
                        name: "current"
                        PropertyChanges { target: view1; x: 0 }
                    },
                    State {
                        name: "exitLeft"
                        PropertyChanges { target: view1; x: -root.width }
                    },
                    State {
                        name: "exitRight"
                        PropertyChanges { target: view1; x: root.width }
                    }
                ]
                transitions: [
                    Transition {
                        to: "current"
                        SequentialAnimation {
                            NumberAnimation { properties: "x"; duration: 250 }
                        }
                    },
                    Transition {
                        NumberAnimation { properties: "x"; duration: 250 }
                        NumberAnimation { properties: "x"; duration: 250 }
                    }
                ]
                Keys.onPressed: root.keyPressed(event.key)
            }


            Keys.onPressed: {
                root.keyPressed(event.key);
                if (event.key === Qt.Key_Return || event.key === Qt.Key_Select || event.key === Qt.Key_Right) {
                    view.currentItem.launch();
                    event.accepted = true;
                } else if (event.key === Qt.Key_Left) {
                    up();
                }
            }



            function goDown(path) {
               // view.x = root.width;
               // view.state = "current";
               // view.focus = true;
                folders.folder = path;
            }

            /*function up() {
                var path = folders.parentFolder;
                if (path.toString().length === 0 || path.toString() === 'file:')
                    return;
                if (folders == folders1) {
                    view = view2
                    folders = folders2;
                    view1.state = "exitRight";
                } else {
                    view = view1
                    folders = folders1;
                    view2.state = "exitRight";
                }
                view.x = -root.width;
                view.state = "current";
                view.focus = true;
                folders.folder = path;
            }

            function keyPressed(key) {
                switch (key) {
                    case Qt.Key_Up:
                    case Qt.Key_Down:
                    case Qt.Key_Left:
                    case Qt.Key_Right:
                        root.showFocusHighlight = true;
                    break;
                    default:
                        // do nothing
                    break;
                }
            }*/
        }
    }
}













































/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
