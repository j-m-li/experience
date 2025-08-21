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

import java.util.*
import javax.mail.*
import javax.mail.internet.InternetAddress
import javax.mail.internet.MimeMessage


class SendMail(
    //private val context: Context,
    private val email: String,
    private val subject: String,
    private val message: String,
    private val host: String,
    private val port: String,
    private val user: String,
    private val password: String,
    private val from: String
) {
    fun doit(): Any? {
        val props = Properties()
        props["mail.smtp.host"] = host
        props["mail.smtp.socketFactory.port"] = port
        props["mail.smtp.socketFactory.class"] = "javax.net.ssl.SSLSocketFactory"
        props["mail.smtp.auth"] = "true"
        props["mail.smtp.port"] = port
        try {
            val session = Session.getDefaultInstance(props)
            val mm = MimeMessage(session)
            mm.setFrom(InternetAddress(from))
            mm.addRecipient(Message.RecipientType.TO, InternetAddress(email))
            mm.subject = subject
            mm.setText(message)
            Transport.send(mm, user, password)
        } catch (e: MessagingException) {
            e.printStackTrace()
            MainFragment.log = "Send message failed."
        }
        return null
    }
}