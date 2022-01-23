import QtQuick          2.12
import QtQuick.Window   2.12
import QtQuick.Layouts  1.12
import QtQuick.Controls 2.12

Window {
    visible:    true
    width:      640
    height:     480

    ColumnLayout {
        anchors.fill:   parent
        spacing:        10
        anchors.top:    parent.top
        anchors.left:   parent.left

        Text {
            Layout.fillWidth:   true
            text:               "Current geographic features CSV file: " + geoFeatureParser.filePath
            wrapMode:           Text.WordWrap
        }

        Text {
            Layout.fillWidth:   true
            text:               "Geographic features:"
            wrapMode:           Text.WordWrap
        }

        ListView {
            Layout.fillWidth:   true
            Layout.fillHeight:  true
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior:     Flickable.StopAtBounds
            clip:               true
            model:              geoFeatureParser.geoFeatures
            ScrollBar.vertical: ScrollBar {}

            delegate: Rectangle {
                width:  parent.width
                height: geoFatureText.height + 20
                color:  index % 2 == 0 ? "white" : "lightgrey"

                Text {
                    id:                 geoFatureText
                    width:              parent.width - 20
                    text:               modelData
                    wrapMode:           Text.WordWrap
                    anchors.centerIn:   parent
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button {
                text: "Sort by name"
                onClicked: geoFeatureParser.sortByName()
            }

            Button {
                text: "Sort by type"
                onClicked: geoFeatureParser.sortByType()
            }

            Button {
                text: "Sort by location"
                onClicked: geoFeatureParser.sortByLocation()
            }
        }
    }
}
