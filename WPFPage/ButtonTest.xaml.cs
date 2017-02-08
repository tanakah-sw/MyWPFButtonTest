using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Animation;
using System.Windows.Interop;
using System.Runtime.InteropServices;

namespace WPFPage
{
    public partial class ButtonTest : UserControl
    {
        [DllImport("user32.dll")]
        public static extern int SendMessage(int hWnd, int msg, int wParam, IntPtr lParam);

        public ButtonTest()
        {
            InitializeComponent();
            Loaded+=ButtonTest_Loaded;
        }

        private void ButtonTest_Loaded(object sender, RoutedEventArgs e)
        {
            var sb=(Storyboard)PodClock.FindResource("sb");
            sb.Begin(PodClock, HandoffBehavior.SnapshotAndReplace, true);
        }

        private void btnclk(object sender, RoutedEventArgs e)
        {
            Button clicked=(Button)sender;
            HwndSource source=(HwndSource)HwndSource.FromVisual(this);
            IntPtr handle=source.Handle;
            SendMessage(handle.ToInt32(), 0x0111, 0, (IntPtr)0);
        }
    }
}