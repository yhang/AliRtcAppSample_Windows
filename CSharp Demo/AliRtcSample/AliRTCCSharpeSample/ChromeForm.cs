using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using CefSharp;
using CefSharp.WinForms;

namespace AliRtcSample
{
    

    public partial class ChromeForm : Form
    {
        public ChromiumWebBrowser browser;
        public ChromeForm()
        {
            InitializeComponent();
            InitBrowser();
        }

        public void InitBrowser()
        {
            CefSettings settings = new CefSettings();
            settings.CachePath = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData) + @"\CEF";
            Cef.Initialize(settings);
            browser = new ChromiumWebBrowser("www.chsi.com.cn");
            this.Controls.Add(browser);
            browser.Dock = DockStyle.Fill;
        }

        private void browser_FrameLoadEnd(object sender, CefSharp.FrameLoadEndEventArgs e)
        {
            var cookieManager = CefSharp.Cef.GetGlobalCookieManager();

            CookieVisitor visitor = new CookieVisitor();
            visitor.SendCookie += visitor_SendCookie;
            cookieManager.VisitAllCookies(visitor);
        }

        public static string cookies;
        private void visitor_SendCookie(CefSharp.Cookie obj)
        {
            cookies += obj.Domain.TrimStart('.') + "^" + obj.Name + "^" + obj.Value + "$";
        }

        public static CookieCollection GetCookieCollection()
        {
            CookieCollection cookieCollection = new CookieCollection();
            string[] arr = cookies.Split('$');
            foreach (string s in arr)
            {
                if (string.IsNullOrEmpty(s))
                    continue;
                string[] car = s.Split('^');
                System.Net.Cookie cookie = new System.Net.Cookie();
                cookie.Domain = car[0];
                cookie.Name = car[1];
                cookie.Value = car[2];
                cookieCollection.Add(cookie);
            }
            return cookieCollection;
        }
    }

    public class CookieVisitor : CefSharp.ICookieVisitor
    {
        public event Action<CefSharp.Cookie> SendCookie;

        public void Dispose()
        {
            //throw new NotImplementedException();
        }

        public bool Visit(CefSharp.Cookie cookie, int count, int total, ref bool deleteCookie)
        {
            deleteCookie = false;
            if (SendCookie != null)
            {
                SendCookie(cookie);
            }

            return true;
        }
    }
}
