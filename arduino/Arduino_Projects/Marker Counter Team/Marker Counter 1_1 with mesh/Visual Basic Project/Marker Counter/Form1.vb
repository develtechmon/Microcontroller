
Imports System
Imports System.IO.Ports

Public Class Form1
    Dim comport As String
    Dim receiveddata As String = ""
    Dim timer = 1
    Dim connected As Boolean
    Dim arduino_state As Integer = 0
    Dim discon_con As Integer = 0
    Dim initializearduino As Integer = 0

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        Timer1.Enabled = False
        Timer3.Start()
        comport = " "

    End Sub

    Private Sub comPort_ComboBox_SelectedIndexChanged(sender As Object, e As EventArgs) Handles comPort_ComboBox.SelectedIndexChanged
        If (comPort_ComboBox.SelectedItem <> "") Then
            comport = comPort_ComboBox.SelectedItem
        End If
    End Sub

    Private Sub Timer3_Tick(sender As Object, e As EventArgs) Handles Timer3.Tick
        Auto_Detect_Arduino_port()
        If (discon_con = 1) Then
            Status.Text = "Connected"
            discon_con = 0
            Timer3.Stop()
        Else
            Status.Text = "Disconnected"
            SerialPort1.Close()
            ''Auto_Detect_Arduino_port()
            Timer3.Stop()
        End If
    End Sub

    Private Sub Connect_Click(sender As Object, e As EventArgs) Handles Refresh_Port.Click
        Timer3.Start()
        ArduinoPort.Text = " "
        TextBox1.Clear()
        RichTextBox1.Clear()
        arduino_state = 0
        SerialPort1.DtrEnable = True
    End Sub

    Sub Auto_Detect_Arduino_port()
        ''*********Auto Detect Arduino Port*************
        For Each sp As String In My.Computer.Ports.SerialPortNames
            Try
                Using com As IO.Ports.SerialPort = My.Computer.Ports.OpenSerialPort(sp)
                    ''comPort_ComboBox.Items.Add(sp)
                    com.ReadTimeout = 3000
                    com.WriteLine("t")
                    Dim data As String = com.ReadLine()

                    ''*****Successfull Handshake*******
                    If (arduino_state = 0) And InStr(data, "OK") Then
                        comport = sp
                        connected = True
                        ArduinoPort.Text = comport
                        arduino_state = 1
                        discon_con = 1
                    End If
                    com.Close()
                End Using
            Catch ex As Exception
            End Try
        Next
    End Sub

    Sub Initialize_Arduino()
        If (Connect.Text = "Receiver Mode Off") And arduino_state = 1 Then
            If (comport <> "") Then
                SerialPort1.Close()
                SerialPort1.PortName = comport
                SerialPort1.BaudRate = 9600
                SerialPort1.DataBits = 8
                SerialPort1.Parity = Parity.None
                SerialPort1.StopBits = StopBits.One
                SerialPort1.Handshake = Handshake.None
                SerialPort1.Encoding = System.Text.Encoding.Default
                SerialPort1.ReadTimeout = 10000
                Connect.Text = "Receiver Mode On"
                initializearduino = 1

                ''SerialPort1.DtrEnable = True
                TextBox1.Text = 0
                ''Timer1.Enabled = True
                SerialPort1.Open()
                arduino_state = 0
            Else
                MsgBox("No Arduino Port Found !")
            End If
        Else
            SerialPort1.Close()
            Connect.Text = "Receiver Mode Off"
            Timer1.Enabled = False
        End If
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Connect.Click

        Initialize_Arduino()

    End Sub

    Private Sub Start_Click(sender As Object, e As EventArgs) Handles Start.Click

        If (Status.Text = "Connected" And timer = 1 And initializearduino = 1) Then
            Timer2.Start()
            Timer1.Enabled = True
            Start.BackgroundImage = My.Resources._Stop
            Start.ImageAlign = ContentAlignment.MiddleCenter
            Start.FlatStyle = FlatStyle.Popup
            Start.Anchor = AnchorStyles.Left
            Start.Anchor = AnchorStyles.Right
            Start.Anchor = AnchorStyles.Top
            Start.Anchor = AnchorStyles.Bottom
            Start.BackgroundImageLayout = ImageLayout.Zoom
            Start.AutoSizeMode = AutoSizeMode.GrowAndShrink
            TextBox1.Text = 0
            ''Auto_Detect_Arduino_port()

            timer = 0

        ElseIf (Status.Text = "Connected" And timer = 0 And initializearduino = 1) Then
            Timer2.Stop()
            Timer1.Enabled = False
            Start.BackgroundImage = My.Resources.red_start_png_5
            Start.ImageAlign = ContentAlignment.MiddleCenter
            Start.FlatStyle = FlatStyle.Popup
            Start.Anchor = AnchorStyles.Left
            Start.Anchor = AnchorStyles.Right
            Start.Anchor = AnchorStyles.Top
            Start.Anchor = AnchorStyles.Bottom
            Start.BackgroundImageLayout = ImageLayout.Zoom
            Start.AutoSizeMode = AutoSizeMode.GrowAndShrink
            timer = 1
            SerialPort1.DtrEnable = True
            ''Auto_Detect_Arduino_port()
        Else
            MsgBox("Please select the Mode First !")
        End If

    End Sub

    Private Sub Timer1_Tick(sender As Object, e As EventArgs) Handles Timer1.Tick
        receiveddata = Receiveserialdata()
        RichTextBox1.Text = receiveddata
        RichTextBox1.SelectionAlignment = HorizontalAlignment.Center
    End Sub

    Function Receiveserialdata() As String
        Dim incoming As String
        Try
            incoming = SerialPort1.ReadExisting()
            If incoming Is Nothing Then
                Return "Nothing !" & vbCrLf
            Else
                Return incoming
            End If
        Catch ex As Exception
            Return "Error Message : Can't read Serial Port"
        End Try
    End Function

    Private Sub Timer2_Tick(sender As Object, e As EventArgs) Handles Timer2.Tick
        Try
            TextBox1.Text = TextBox1.Text + 1
        Catch ex As Exception

        End Try
    End Sub


End Class
