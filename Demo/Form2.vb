Public Class Form2

    Private Sub Form2_FormClosed(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosedEventArgs) Handles Me.FormClosed
        Me.Dispose()
    End Sub

    Private Sub Form2_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Label3.Text += Form1.md5list(Form1.ci) + ".hcert"
        Label4.Text += Form1.md5list(Form1.ci) + ".hkey"
    End Sub

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        Button1.Visible = False
        Button2.Visible = True
        Dim p As New Process
        p.StartInfo.FileName = "HKeyChallenge.exe"
        p.StartInfo.Arguments = "-s """ + Form1.path + "\start.txt"" """ + Form1.flist(Form1.ci) + """"
        p.StartInfo.UseShellExecute = False
        p.StartInfo.RedirectStandardOutput = True
        p.StartInfo.CreateNoWindow = False
        p.Start()
        p.WaitForExit()
        If Not My.Computer.FileSystem.FileExists(Form1.path + "\start.txt") Then
            MsgBox("没有找到输出文件！")
            Me.Dispose()
            Exit Sub
        End If
        Dim fs As New IO.FileStream(Form1.path + "\start.txt", IO.FileMode.Open)
        Dim tr As New IO.StreamReader(fs)
        Dim ts As String = tr.ReadToEnd
        tr.Close()
        fs.Close()
        If InStr(ts, ".") <= 0 Then
            MsgBox("成功！" + vbCrLf + ts)
        Else
            MsgBox("失败！" + vbCrLf + ts)
            Me.Dispose()
        End If
    End Sub

    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
        Button2.Visible = False
        Button3.Visible = True
        Dim p As New Process
        p.StartInfo.FileName = "HKeyRespond.exe"
        p.StartInfo.Arguments = "-a """ + Form1.path + "\newgraph.txt"" """ + Form1.path + "\start.txt"" """ + Form1.TextBox1.Text + """"
        p.StartInfo.UseShellExecute = False
        p.StartInfo.RedirectStandardOutput = True
        p.StartInfo.CreateNoWindow = False
        p.Start()
        p.WaitForExit()
        If Not My.Computer.FileSystem.FileExists(Form1.path + "\newgraph.txt") Then
            MsgBox("没有找到输出文件！")
            Me.Dispose()
            Exit Sub
        End If
        Dim ts As String = p.StandardOutput.ReadToEnd
        If InStr(ts, ".") <= 0 Then
            MsgBox("成功！" + vbCrLf + ts)
        Else
            MsgBox("失败！" + vbCrLf + ts)
            Me.Dispose()
        End If
    End Sub

    Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.Click
        Button3.Visible = False
        Button4.Visible = True
        Dim p As New Process
        p.StartInfo.FileName = "HKeyChallenge.exe"
        p.StartInfo.Arguments = "-c """ + Form1.path + "\sequence.txt"" """ + Form1.flist(Form1.ci) + """ """ + Form1.path + "\newgraph.txt"""
        p.StartInfo.UseShellExecute = False
        p.StartInfo.RedirectStandardOutput = True
        p.StartInfo.CreateNoWindow = False
        p.Start()
        p.WaitForExit()
        If Not My.Computer.FileSystem.FileExists(Form1.path + "\sequence.txt") Then
            MsgBox("没有找到输出文件！")
            Me.Dispose()
            Exit Sub
        End If
        Dim fs As New IO.FileStream(Form1.path + "\sequence.txt", IO.FileMode.Open)
        Dim tr As New IO.StreamReader(fs)
        Dim ts As String = tr.ReadToEnd
        tr.Close()
        If InStr(ts, ".") <= 0 Then
            MsgBox("成功！" + vbCrLf + ts)
        Else
            MsgBox("失败！" + vbCrLf + ts)
            Me.Dispose()
        End If
    End Sub

    Private Sub Button4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button4.Click
        Button4.Visible = False
        Button5.Visible = True
        Dim p As New Process
        p.StartInfo.FileName = "HKeyRespond.exe"
        p.StartInfo.Arguments = "-b """ + Form1.path + "\answer.txt"" """ + Form1.path + "\sequence.txt"" """ + Form1.TextBox1.Text + """"
        p.StartInfo.UseShellExecute = False
        p.StartInfo.RedirectStandardOutput = True
        p.StartInfo.CreateNoWindow = False
        p.Start()
        p.WaitForExit()
        If Not My.Computer.FileSystem.FileExists(Form1.path + "\answer.txt") Then
            MsgBox("没有找到输出文件！")
            Me.Dispose()
            Exit Sub
        End If
        Dim ts As String = p.StandardOutput.ReadToEnd
        If InStr(ts, ".") <= 0 Then
            MsgBox("成功！" + vbCrLf + ts)
        Else
            MsgBox("失败！" + vbCrLf + ts)
            Me.Dispose()
        End If
    End Sub

    Private Sub Button5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button5.Click
        Dim p As New Process
        p.StartInfo.FileName = "HKeyChallenge.exe"
        p.StartInfo.Arguments = "-v """ + Form1.path + "\final.txt"" """ + Form1.flist(Form1.ci) + """ """ + Form1.path + "\newgraph.txt"" """ + Form1.path + "\answer.txt"""
        p.StartInfo.UseShellExecute = False
        p.StartInfo.RedirectStandardOutput = True
        p.StartInfo.CreateNoWindow = False
        p.Start()
        p.WaitForExit()
        If Not My.Computer.FileSystem.FileExists(Form1.path + "\sequence.txt") Then
            MsgBox("没有找到输出文件！")
            Me.Dispose()
            Exit Sub
        End If
        Dim fs As New IO.FileStream(Form1.path + "\final.txt", IO.FileMode.Open)
        Dim tr As New IO.StreamReader(fs)
        Dim ts As String = tr.ReadToEnd
        tr.Close()
        If InStr(ts, "OK") > 0 Then
            MsgBox("成功！" + vbCrLf + ts)
        Else
            MsgBox("失败！" + vbCrLf + ts)
            Me.Dispose()
        End If
    End Sub
End Class