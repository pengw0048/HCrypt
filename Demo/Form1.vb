Public Class Form1

    Public path As String
    Public flist As New List(Of String), md5list As New List(Of String)
    Public ci As Integer

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        path = IIf(Application.StartupPath.EndsWith("\"), Application.StartupPath.Remove(Application.StartupPath.Length - 1), Application.StartupPath)
        TextBox1.Text = path
        updateList()
    End Sub

    Sub updateList()
        ListBox1.Items.Clear()
        flist.Clear()
        md5list.Clear()
        If TextBox1.Text.EndsWith("\") Then TextBox1.Text = TextBox1.Text.Remove(TextBox1.Text.Length - 1)
        If Not My.Computer.FileSystem.DirectoryExists(TextBox1.Text + "\cert") Then Exit Sub
        For Each file In My.Computer.FileSystem.GetFiles(TextBox1.Text + "\cert")
            Dim ts() As String = Split(file, "\"), s As String = "", fpath As String = file
            flist.Add(fpath)
            file = ts(UBound(ts))
            If My.Computer.FileSystem.FileExists(TextBox1.Text + "\key\" + file.Remove(32) + ".hkey") Then s = "(有私钥)"
            s += file.Remove(32)
            md5list.Add(file.Remove(32))
            s += ":" + shortstr(readfile(fpath))
            ListBox1.Items.Add(s)
        Next
    End Sub

    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
        FolderBrowserDialog1.ShowDialog()
        TextBox1.Text = FolderBrowserDialog1.SelectedPath
        updateList()
    End Sub

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        If Not My.Computer.FileSystem.DirectoryExists(TextBox1.Text) Then Exit Sub
        If Not My.Computer.FileSystem.DirectoryExists(TextBox1.Text + "\cert") Then My.Computer.FileSystem.CreateDirectory(TextBox1.Text + "\cert")
        If Not My.Computer.FileSystem.DirectoryExists(TextBox1.Text + "\key") Then My.Computer.FileSystem.CreateDirectory(TextBox1.Text + "\key")
        OpenFileDialog1.Title = "选择附加内容文件"
        OpenFileDialog1.ShowDialog()
        If Not My.Computer.FileSystem.FileExists(OpenFileDialog1.FileName) Then Exit Sub
        Dim p As New Process
        p.StartInfo.FileName = "HKeygen.exe"
        p.StartInfo.Arguments = """" + OpenFileDialog1.FileName + """ """ + path + "\tmp"""
        p.StartInfo.UseShellExecute = False
        p.StartInfo.RedirectStandardOutput = True
        p.StartInfo.CreateNoWindow = False
        p.Start()
        p.WaitForExit()
        Dim out As String = p.StandardOutput.ReadToEnd
        Dim res() As String = Split(out, "Trait: ")
        If UBound(res) = 0 Then
            MsgBox("失败！" + vbCrLf + out)
        Else
            Dim md5 As String = Strings.Left(res(1), 32)
            My.Computer.FileSystem.MoveFile(path + "\tmp.hcert", TextBox1.Text + "\cert\" + md5 + ".hcert")
            My.Computer.FileSystem.MoveFile(path + "\tmp.hkey", TextBox1.Text + "\key\" + md5 + ".hkey")
            MsgBox("成功！" + vbCrLf + out)
        End If
        updateList()
    End Sub

    Function readfile(ByVal filename As String) As String
        Dim fs As New IO.FileStream(filename, IO.FileMode.Open)
        Dim ret As String = ""
        Dim bs As New IO.BinaryReader(fs)
        bs.BaseStream.Seek(9, IO.SeekOrigin.Begin)
        Dim len As Integer = CInt(bs.ReadByte) * 256 + CInt(bs.ReadByte)
        bs.BaseStream.Seek(len, IO.SeekOrigin.Current)
        len = CInt(bs.ReadByte) * 256 + CInt(bs.ReadByte)
        ret = bs.ReadChars(len)
        fs.Close()
        Return ret
    End Function

    Function shortstr(ByVal tstr As String) As String
        If tstr.Length < 50 Then Return tstr
        Return tstr.Remove(50)
    End Function

    Private Sub ListBox1_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ListBox1.SelectedIndexChanged
        If ListBox1.SelectedIndex < 0 Then Exit Sub
        If ListBox1.SelectedItem.ToString.StartsWith("(") Then
            Button3.Enabled = True
            Button5.Enabled = True
        Else
            Button3.Enabled = False
            Button5.Enabled = False
        End If
    End Sub

    Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.Click
        If ListBox1.SelectedIndex < 0 Then Exit Sub
        OpenFileDialog1.Title = "选择要签名的文件"
        OpenFileDialog1.ShowDialog()
        If Not My.Computer.FileSystem.FileExists(OpenFileDialog1.FileName) Then Exit Sub
        Dim p As New Process
        p.StartInfo.FileName = "HKeySign.exe"
        Dim idx As Integer = ListBox1.SelectedIndex
        p.StartInfo.Arguments = """" + OpenFileDialog1.FileName + ".hsign" + """ """ + OpenFileDialog1.FileName + """ """ + flist(idx) + """ """ + flist(idx).Remove(flist(idx).Length - 43) + "key\" + md5list(idx) + ".hkey"""
        p.StartInfo.UseShellExecute = False
        p.StartInfo.RedirectStandardOutput = True
        p.StartInfo.CreateNoWindow = False
        p.Start()
        p.WaitForExit()
        Dim out As String = p.StandardOutput.ReadLine
        If Trim(out) <> "" Then
            MsgBox("失败！" + vbCrLf + out)
        Else
            MsgBox("成功！" + vbCrLf + out)
        End If
    End Sub

    Private Sub Button4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button4.Click
        OpenFileDialog1.Title = "选择要验证的文件"
        OpenFileDialog1.ShowDialog()
        If Not My.Computer.FileSystem.FileExists(OpenFileDialog1.FileName) Then Exit Sub
        If Not My.Computer.FileSystem.FileExists(OpenFileDialog1.FileName + ".hsign") Then
            MsgBox("没有签名文件")
            Exit Sub
        End If
        Dim p As New Process
        p.StartInfo.FileName = "HKeyVerify.exe"
        Dim idx As Integer = ListBox1.SelectedIndex
        p.StartInfo.Arguments = """" + OpenFileDialog1.FileName + """ """ + OpenFileDialog1.FileName + ".hsign"" """ + TextBox1.Text + """"
        p.StartInfo.UseShellExecute = False
        p.StartInfo.RedirectStandardOutput = True
        p.StartInfo.CreateNoWindow = False
        p.Start()
        p.WaitForExit()
        Dim out As String = p.StandardOutput.ReadToEnd
        If InStr(out, "OK") <= 0 Then
            MsgBox("失败！" + vbCrLf + out)
        Else
            MsgBox("成功！" + vbCrLf + out)
        End If
    End Sub

    Private Sub Button6_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button6.Click
        If ListBox1.SelectedIndex < 0 Then Exit Sub
        Dim idx As Integer = ListBox1.SelectedIndex
        My.Computer.FileSystem.DeleteFile(flist(idx))
        My.Computer.FileSystem.DeleteFile(flist(idx).Remove(flist(idx).Length - 43) + "key\" + md5list(idx) + ".hkey")
        updateList()
    End Sub

    Private Sub Button5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button5.Click
        If ListBox1.SelectedIndex < 0 Then Exit Sub
        ci = ListBox1.SelectedIndex
        Form2.ShowDialog()
    End Sub
End Class
