/*
 *          PUBLIC DOMAIN 20 August MMXXIII by O'ksi'D
 *
 *        The authors disclaim copyright to this software.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a
 * compiled binary, for any purpose, commercial or non-commercial,
 * and by any means.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT OF ANY PATENT, COPYRIGHT, TRADE SECRET OR OTHER
 * PROPRIETARY RIGHT.  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

package com.cod5.dmwp

import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.EditText
import android.widget.ProgressBar
import androidx.datastore.preferences.core.stringPreferencesKey
import com.cod5.dmwp.databinding.FragmentMainBinding
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.flow.map
import kotlinx.coroutines.runBlocking
import java.io.File
import java.util.Date
import java.util.LinkedList

import kotlin.collections.HashMap
import java.util.Timer
import kotlin.concurrent.timerTask

class MainFragment : Fragment(), View.OnClickListener {
    private var _binding: FragmentMainBinding? = null
    private val binding get() = _binding!!

    companion object {
        private var email: String = ""
        private var subject: String = ""
        private var message: String = ""
        var cfg = HashMap<Int, String>()
        private val qu = LinkedList<Pair<String, String>>()
        var tim: Timer? = null
        var log: String = ""
        private var delay: Long = 1
        var isRunning: Boolean = false

        fun check() {
            isRunning = true
            if (delay <= 0) {
                val rm = ReadMail(
                    cfg[R.id.editTextImap]!!,
                    cfg[R.id.editTextImapPort]!!,
                    cfg[R.id.editTextImapUser]!!,
                    cfg[R.id.editTextImapPassword]!!,
                    qu
                )
                rm.execute()

                var l = log.length
                if (l > 50) {
                    l -= 50
                } else {
                    l = 0
                }
                log = log.substring(l) + Date().toString() + "\n"
                //Log.d("lo", log)
                while (qu.isNotEmpty()) {
                    val it = qu.removeFirst()
                    log = log + it.first + it.second + "\n"
                    if (it.second.startsWith("GET /") || it.second.startsWith("PUT /")) {
                        var idx = it.second.indexOf(' ', 5)
                        if (idx < 5) idx = it.second.length
                        val url = it.second.substring(5, idx)
                        if (!url.contains("..")) {
                            try {
                                email = it.first
                                if (url.isEmpty()) {
                                    val li = MainActivity.path.listFiles { _ -> true }
                                    message = "LIST"
                                    subject = "RE: " + it.second
                                    li?.forEach {
                                        message =
                                            message + "\nmailto:" + cfg[R.id.editTextSender]!! + "?subject=GET%20/" + it.name
                                    }
                                    sendEmail()
                                    continue
                                }
                                val f = File(MainActivity.path, url)
                                if (it.second.startsWith("G")) {
                                    subject = "RE: GET /$url"
                                    message = f.readText(Charsets.US_ASCII)
                                    sendEmail()
                                } else if (!f.exists() && url.endsWith(".txt")) {
                                    subject = "RE: PUT /$url"
                                    f.writeText(it.second.substring(idx), Charsets.US_ASCII)
                                    message = "File saved: $url"
                                    sendEmail()
                                }

                            } catch (e: Exception) {
                                e.printStackTrace()
                            }
                        }
                    }
                }

                Log.d("JML", "check out")
                delay = try {
                    cfg[R.id.editTextPoll]!!.toLong()
                } catch (e: Exception) {
                    30
                }
            }
            delay--
            try {
                tim!!.schedule(timerTask { check() }, 1000)
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }

        private fun sendEmail() {
            val sm = SendMail(
                email, subject, message,
                cfg[R.id.editTextSmtp]!!,
                cfg[R.id.editTextSmtpPort]!!,
                cfg[R.id.editTextSmtpUser]!!,
                cfg[R.id.editTextSmtpPassword]!!,
                cfg[R.id.editTextSender]!!
            )
            sm.doit()
        }
    }

    override fun onClick(v: View?) {

    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        _binding = FragmentMainBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        //buttonSend = binding.root.findViewById<View>(R.id.buttonSend) as Button
        restore()
        //buttonSend!!.setOnClickListener(this)
        /*binding.buttonFirst.setOnClickListener {
            findNavController().navigate(R.id.action_FirstFragment_to_SecondFragment)
        }*/
    }

    override fun onStart() {
        super.onStart()
        try {
            tim = Timer()
            Timer().schedule(timerTask { logIt() }, 5000)
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }

    private fun logIt() {
        if (_binding != null) {
            val l = binding.root.findViewById<View>(R.id.editTextMessage) as EditText
            val r = binding.root.findViewById<View>(R.id.progressBar) as ProgressBar
            if (isRunning) {
                isRunning = false
                r.post { r.visibility = ProgressBar.VISIBLE }
            } else {
                r.post { r.visibility = ProgressBar.INVISIBLE }
            }
            l.post { l.setText(log) }
        }
        Timer().schedule(timerTask { logIt() }, 1200)
    }


    private fun restore() {
        val pref = MainActivity.getPref()
        cfg.clear()
        cfg[R.id.editTextImap] = ""
        cfg[R.id.editTextImapPort] = ""
        cfg[R.id.editTextImapUser] = ""
        cfg[R.id.editTextImapPassword] = ""
        cfg[R.id.editTextSmtp] = ""
        cfg[R.id.editTextSmtpPort] = ""
        cfg[R.id.editTextSmtpUser] = ""
        cfg[R.id.editTextSmtpPassword] = ""
        cfg[R.id.editTextSender] = ""
        cfg[R.id.editTextPoll] = ""

        cfg.forEach {
            val xx: Flow<String> = pref.data.map { preferences ->
                preferences[stringPreferencesKey(it.key.toString())] ?: ""
            }
            runBlocking { cfg[it.key] = xx.first() }
        }
        ReadMail.session = null
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}