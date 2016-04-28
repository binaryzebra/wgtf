﻿///////////////////////////////////////////////////////////////////////////////
//
// This file was automatically generated by RANOREX.
// DO NOT MODIFY THIS FILE! It is regenerated by the designer.
// All your modifications will be lost!
// http://www.ranorex.com
//
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Drawing;
using System.Threading;
using WinForms = System.Windows.Forms;

using Ranorex;
using Ranorex.Core;
using Ranorex.Core.Testing;
using Ranorex.Core.Repository;

namespace NGTTestAutomation.Generic_app_UI
{
#pragma warning disable 0436 //(CS0436) The type 'type' in 'assembly' conflicts with the imported type 'type2' in 'assembly'. Using the type defined in 'assembly'.
    /// <summary>
    ///The LFT_Item_selection recording.
    /// </summary>
    [TestModule("4d2ddd2d-e70d-437a-8ed6-1b436697f806", ModuleType.Recording, 1)]
    public partial class LFT_Item_selection : ITestModule
    {
        /// <summary>
        /// Holds an instance of the NGTTestAutomation.NGTTestAutomationRepository repository.
        /// </summary>
        public static NGTTestAutomation.NGTTestAutomationRepository repo = NGTTestAutomation.NGTTestAutomationRepository.Instance;

        static LFT_Item_selection instance = new LFT_Item_selection();

        /// <summary>
        /// Constructs a new instance.
        /// </summary>
        public LFT_Item_selection()
        {
        }

        /// <summary>
        /// Gets a static instance of this recording.
        /// </summary>
        public static LFT_Item_selection Instance
        {
            get { return instance; }
        }

#region Variables

#endregion

        /// <summary>
        /// Starts the replay of the static recording <see cref="Instance"/>.
        /// </summary>
        [System.CodeDom.Compiler.GeneratedCode("Ranorex", "5.4.5")]
        public static void Start()
        {
            TestModuleRunner.Run(Instance);
        }

        /// <summary>
        /// Performs the playback of actions in this recording.
        /// </summary>
        /// <remarks>You should not call this method directly, instead pass the module
        /// instance to the <see cref="TestModuleRunner.Run(ITestModule)"/> method
        /// that will in turn invoke this method.</remarks>
        [System.CodeDom.Compiler.GeneratedCode("Ranorex", "5.4.5")]
        void ITestModule.Run()
        {
            Mouse.DefaultMoveTime = 300;
            Keyboard.DefaultKeyPressTime = 100;
            Delay.SpeedFactor = 1.0;

            Init();

            Report.Log(ReportLevel.Info, "Mouse", "Mouse Left Click item 'MainWindow.LFT_panel.WGListViewRowDelegate' at Center.", repo.MainWindow.LFT_panel.WGListViewRowDelegateInfo, new RecordItemIndex(0));
            repo.MainWindow.LFT_panel.WGListViewRowDelegate.Click();
            Delay.Milliseconds(200);
            
            Report.Log(ReportLevel.Info, "Keyboard", "Key sequence '{LShiftKey down}'.", new RecordItemIndex(1));
            Keyboard.Press("{LShiftKey down}");
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Mouse", "Mouse Left Click item 'MainWindow.LFT_panel.WGListViewRowDelegate4' at Center.", repo.MainWindow.LFT_panel.WGListViewRowDelegate4Info, new RecordItemIndex(2));
            repo.MainWindow.LFT_panel.WGListViewRowDelegate4.Click();
            Delay.Milliseconds(200);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating CompareImage (Screenshot: 'Screenshot7' with region {X=0,Y=0,Width=1174,Height=922}) on item 'MainWindow.LFT_panel.List'.", repo.MainWindow.LFT_panel.ListInfo, new RecordItemIndex(3));
            Validate.CompareImage(repo.MainWindow.LFT_panel.ListInfo, List_Screenshot7, List_Screenshot7_Options);
            Delay.Milliseconds(0);
            
        }

#region Image Feature Data
        CompressedImage List_Screenshot7
        { get { return repo.MainWindow.LFT_panel.ListInfo.GetScreenshot7(new Rectangle(0, 0, 1174, 922)); } }

        Imaging.FindOptions List_Screenshot7_Options
        { get { return Imaging.FindOptions.Parse("0.8;EdgesSobel;0,0,1174,922;True;10000000;0ms"); } }

#endregion
    }
#pragma warning restore 0436
}
