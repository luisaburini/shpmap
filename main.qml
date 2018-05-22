import QtQuick 2.6
import QtQuick.Window 2.2
import io.qt.shphandler 1.0
import io.qt.snowboywatcher 1.0

Window {

    visible: true
    width: 800
    height: 480
    title: qsTr("Hello Shapefile")

    SHPHandler{
        id: shpHandler
        indexPoints: 0
        indexParts: 0
    }

    SnowboyWatcher{
        id: snowboyWatcher
        snowboyRecog: false
    }

    Timer{
        interval: 500
        running: true
        repeat: true
        onTriggered: {
            if(snowboyWatcher.snowboyRecog)
            {
                fileChangedText.text = "ONE HOTWORD DETECTED"
                fileChangedText.color = Qt.rgba(Math.random(), Math.random(), Math.random(), 1)
                greenLed.source = "qrc:/images/led-green-on.png"
                redLed.source = "qrc:/images/led-red-on.png"
            }
            else
            {
                fileChangedText.text = ""
                greenLed.source = "qrc:/images/led-green-off.png"
                redLed.source = "qrc:/images/led-red-off.png"
            }
            snowboyWatcher.snowboyRecog = false
        }
    }

    Column{
        spacing: 5
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        Image {
            id: greenLed
            source: "qrc:/images/led-green-off.png"
        }
        Image {
            id: redLed
            source: "qrc:/images/led-red-off.png"
        }
    }


    Rectangle{
        anchors.fill: parent
        color: "transparent"

        Text {
            id: dimensions
            text: shapefileMap.width + " " + shapefileMap.height
            color: "white"
            font.pixelSize: 35
        }

        Text{
            id: fileChangedText
            font.pixelSize: 25
            anchors.top: parent.top
            anchors.right: parent.right
            color: "black"
        }

        Text {
            id: plottedPointsText
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            color: "black"
            font.pixelSize: 45
        }

        Item{
            anchors.fill: parent
            LayoutMirroring.enabled: true
            LayoutMirroring.childrenInherit: true
            anchors.centerIn: parent
            Canvas{
                    id: shapefileMap
                    anchors.centerIn: parent
                    width: shpHandler.width*16
                    height: shpHandler.height*16
                    transform: Rotation{ origin.x: 1120; origin.y: 280; angle: -90}

                    onPaint: {

                        var ctx = getContext("2d")
                        ctx.lineWidth = 1
                        ctx.strokeStyle = "black"
                        var zoomFactor = 11
                        var plottedPoints = 0

                        // iterate over the number of parts
                        console.log(shpHandler.numParts)
                        if (shpHandler.numParts == 1)
                        {
                            console.log("TENHO 1 PARTE")
                            var firstX = (shpHandler.longitudeX)*zoomFactor
                            var firstY = (shpHandler.latitudeY)*zoomFactor
                            ctx.beginPath()
                            ctx.moveTo(firstY, firstX)
                            for(shpHandler.indexPoints = 0; shpHandler.indexPoints < shpHandler.totalNumPoints; shpHandler.indexPoints++)
                            {
                                ctx.lineTo(shpHandler.latitudeY*zoomFactor, shpHandler.longitudeX*zoomFactor)
                                plottedPoints++;
                                //console.log("Aqui do QML: LongitudeX ", shpHandler.longitudeX*zoomFactor + " at " + shpHandler.indexPoints)
                                //console.log("Aqui do QML: latitude Y ", shpHandler.latitudeY*zoomFactor + " at " + shpHandler.indexPoints)
                            }
                            ctx.closePath()
                            ctx.stroke()
                            console.log("fui ate o final do loop da figura")
                        }
                        else{
                            console.log("TENHO MUITAS PARTES")
                            console.log(shpHandler.numParts)

                            for(shpHandler.indexParts=0; shpHandler.indexParts < shpHandler.numParts; shpHandler.indexParts++)
                            {
                                ctx.beginPath()
                                var currentPart, nextPart

                                if(shpHandler.indexParts > shpHandler.numParts-2)
                                {
                                    currentPart = shpHandler.numPoints
                                    nextPart = shpHandler.totalNumPoints;
                                }
                                else{
                                    currentPart = shpHandler.numPoints
                                    shpHandler.indexParts++
                                    nextPart = shpHandler.numPoints
                                    shpHandler.indexParts--
                                }

                                firstX = (shpHandler.longitudeX)*zoomFactor
                                firstY = (shpHandler.latitudeY)*zoomFactor
                                ctx.moveTo(firstY, firstX)
                                //console.log("This is current part: " + currentPart)
                                //console.log("This is next part: " + nextPart)
                                for(shpHandler.indexPoints = currentPart; shpHandler.indexPoints < nextPart; shpHandler.indexPoints++)
                                {
                                    ctx.lineTo((shpHandler.latitudeY)*zoomFactor, (shpHandler.longitudeX)*zoomFactor)
                                    plottedPoints++
                                    //console.log("Aqui do QML: LongitudeX ", shpHandler.longitudeX*zoomFactor + " at " + shpHandler.indexPoints)
                                    //console.log("Aqui do QML: latitude Y ", shpHandler.latitudeY*zoomFactor + " at " + shpHandler.indexPoints)
                                }
                                ctx.closePath()
                                ctx.fill()
                                ctx.stroke()
                            }
                        }

                        console.log("I HAVE PLOTTED ", plottedPoints, " POINTS")
                        plottedPointsText.text = "Plotted points: " + plottedPoints.toString()
                    }
                }
        }
    }

}
